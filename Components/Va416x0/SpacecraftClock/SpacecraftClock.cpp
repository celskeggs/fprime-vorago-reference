// Copyright 2026 California Institute of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// ======================================================================
// \title  SpacecraftClock.cpp
// \brief  cpp file for SpacecraftClock component implementation class
// ======================================================================

#include "Components/Va416x0/SpacecraftClock/SpacecraftClock.hpp"

constexpr U32 MICROSECONDS_PER_SECOND = 1000000;
constexpr F32 MICROSECOND_SCALER = 0.992F;

namespace Va416x0 {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

SpacecraftClock ::SpacecraftClock(const char* const compName) : SpacecraftClockComponentBase(compName) {}

void SpacecraftClock ::configure(Fw::Time initialTime, U32 nominalRtiDurationUs, U32 rtisPerSecond) {
    Os::ScopeLock lock(this->m_updateMutex);

    // Should only be used once, during initialization.
    FW_ASSERT(this->m_currentEpoch.load() == NUM_PING_PONG_EPOCHS, this->m_currentEpoch.load());

    this->m_epochs[0] = {initialTime, Os::RawTime(),
                         Va416x0Types::RtiTimeWithValidity(false, Va416x0Types::RtiTime{0, 0})};
    // NOTE: Timestamping in timeGetPort_handler assumes the offsetUs for rtiTime is 0
    // and even though it is explicitly set above, assert it here as well
    FW_ASSERT(this->m_epochs[0].rtiTime.get_rtiTime().get_offsetUs() == 0);
    this->m_currentEpoch.store(0);

    this->m_nominalRtiDurationUs = nominalRtiDurationUs;
    // Prevent divide-by-zero in timeGetPort_handler
    FW_ASSERT(rtisPerSecond > 0);
    this->m_rtisPerSecond = rtisPerSecond;

    // Initialize sclk (FIXME - do we want this to be something other than 0)
    this->tlmWrite_CurrentTime(0);
}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void SpacecraftClock ::timeGetPort_handler(FwIndexType portNum, Fw::Time& time) {
    // This code assumes that the epoch will never be changed by an ISR,
    // so we don't need to worry about the current epoch buffer changing underneath us.
    U32 epochIndex = this->m_currentEpoch.load();
    FW_ASSERT(epochIndex <= NUM_PING_PONG_EPOCHS, epochIndex);

    time = Fw::ZERO_TIME;
    if (epochIndex == NUM_PING_PONG_EPOCHS) {
        return;
    }

    Va416x0Types::RtiTimeWithValidity rtiTime = this->getRtiTime_out(0);

    const Epoch& epoch = this->m_epochs[epochIndex];
    time = epoch.fwTime;

    // If RtiTime data is valid, use it to determine elapsed time
    if (rtiTime.get_isValid()) {
        U32 elapsedRti = rtiTime.get_rtiTime().get_rti() - epoch.rtiTime.get_rtiTime().get_rti();
        // Timestamping logic assumes epoch rtiTime offsetUs is 0
        FW_ASSERT(epoch.rtiTime.get_rtiTime().get_offsetUs() == 0);
        U32 elapsedUs = rtiTime.get_rtiTime().get_offsetUs();
        // Since the metronome can extend an RTI length beyond a
        // nominal RTI length, scale the elapsedUs value
        // to fit within the same 1s major frame; this will ensure
        // the returned time value is monotonic across major frames
        U32 scaledUs = elapsedUs * MICROSECOND_SCALER;
        scaledUs += (elapsedRti % this->m_rtisPerSecond) * this->m_nominalRtiDurationUs;
        time.add((elapsedRti / this->m_rtisPerSecond), scaledUs);
    } else {
        Os::RawTime rawTimeNow;
        // Get raw time at this moment
        Os::RawTime::Status status = rawTimeNow.now();
        if (status != Os::RawTime::OP_OK) {
            return;
        }
        Fw::TimeInterval elapsed;
        // Calculate elapsed time from epoch OS time and raw time
        status = rawTimeNow.getTimeInterval(epoch.osTime, elapsed);
        if (status != Os::RawTime::OP_OK) {
            return;
        }
        // Add elapsed time to fwTime
        time.add(elapsed.getSeconds(), elapsed.getUSeconds());
    }
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void SpacecraftClock ::SET_CURRENT_TIME_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 secondsNow) {
    Os::ScopeLock lock(this->m_updateMutex);

    FW_ASSERT(this->m_currentEpoch.load() <= NUM_PING_PONG_EPOCHS, this->m_currentEpoch.load());
    U32 oppositeEpoch = this->m_currentEpoch.load() == 1 ? 0 : 1;

    Epoch& epoch = this->m_epochs[oppositeEpoch];
    epoch.rtiTime = this->getRtiTime_out(0);
    // Force offsetUs to be 0 to update the rtiTime to the beginning of the RTI
    // Timestamping in timeGetPort_handler asserts offsetUs is 0
    epoch.rtiTime.get_rtiTime().set_offsetUs(0);
    Os::RawTime::Status status = epoch.osTime.now();

    if (status != Os::RawTime::OP_OK) {
        // FIXME: Is this the right error handling for flight?
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
        return;
    }

    epoch.fwTime = Fw::Time(secondsNow, 0);

    this->m_currentEpoch.store(oppositeEpoch);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Va416x0
