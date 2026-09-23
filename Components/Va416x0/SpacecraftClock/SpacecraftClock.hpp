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
// \title  SpacecraftClock.hpp
// \brief  hpp file for SpacecraftClock component implementation class
// ======================================================================

#ifndef Va416x0_SpacecraftClock_HPP
#define Va416x0_SpacecraftClock_HPP

#include "Components/Va416x0/SpacecraftClock/SpacecraftClockComponentAc.hpp"
#include "Os/RawTime.hpp"

#include <atomic>

namespace Va416x0 {

class SpacecraftClock final : public SpacecraftClockComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct SpacecraftClock object
    SpacecraftClock(const char* const compName  //!< The component name
    );

    void configure(Fw::Time initialTime, U32 nominalRtiDurationUs, U32 rtisPerSecond);

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for timeGetPort
    //!
    //! Port to retrieve time
    void timeGetPort_handler(FwIndexType portNum,  //!< The port number
                             Fw::Time& time        //!< Reference to Time object
                             ) override;

    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command SET_CURRENT_TIME
    void SET_CURRENT_TIME_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                     U32 cmdSeq,           //!< The command sequence number
                                     U32 secondsNow) override;

    struct Epoch {
        Fw::Time fwTime;
        Os::RawTime osTime;
        Va416x0Types::RtiTimeWithValidity rtiTime;
    };

    static constexpr U32 NUM_PING_PONG_EPOCHS = 2;

    Os::Mutex m_updateMutex;
    // Note: Updates to these variables are protected by the mutex
    // and by the fact that commands are only processed in the main rategroup,
    // but the time itself can be retrieved even from an ISR.
    Epoch m_epochs[NUM_PING_PONG_EPOCHS];
    // Indicates that the epochs have not yet been configured
    std::atomic<U32> m_currentEpoch = {NUM_PING_PONG_EPOCHS};

    //! Nominal RTI duration used in time interval calcuation
    U32 m_nominalRtiDurationUs;
    //! RTIs per second configured for use in RTI-to-Second conversion
    U32 m_rtisPerSecond;
};

}  // namespace Va416x0

#endif
