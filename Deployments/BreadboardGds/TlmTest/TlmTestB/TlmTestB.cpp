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
// \title  TlmTestB.cpp
// \author root
// \brief  cpp file for TlmTestB component implementation class
// ======================================================================

#include "Deployments/BreadboardGds/TlmTest/TlmTestB/TlmTestB.hpp"
#include "Fw/Tlm/TlmBuffer.hpp"

namespace TlmTest {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmTestB::TlmTestB(const char* const compName) : TlmTestBComponentBase(compName) {}

TlmTestB::~TlmTestB() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void TlmTestB::TRIGGER_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    U8 u8chan;
    U16 u16chan;
    U32 u32chan;
    I8 i8chan;
    I16 i16chan;
    I32 i32chan;
    Fw::String evenString;
    Fw::String oddString;
    TlmTest::U8x3 array;
    TlmTest::StructChan structure;

    Fw::TlmBuffer buffer;
    Fw::Time timestamp;
    Fw::TlmValid valid;
    Fw::SerializeStatus status;

    // NOTE: the channel IDs in the tlmGet_out calls below are derived from the TlmTestA component
    // base ID from BreadboardGds.fpp and the TlmTestA relative channel IDs from TlmTestA.fpp

    valid = this->tlmGet_out(0, 0x0240, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(u8chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0241, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(u16chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0242, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(u32chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0243, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(i8chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0244, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(i16chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0245, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = buffer.deserializeTo(i32chan);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0246, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = evenString.deserializeFrom(buffer);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0247, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = oddString.deserializeFrom(buffer);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0248, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = array.deserializeFrom(buffer);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    valid = this->tlmGet_out(0, 0x0249, timestamp, buffer);
    FW_ASSERT(valid == Fw::TlmValid::VALID, valid);
    status = structure.deserializeFrom(buffer);
    FW_ASSERT(status == Fw::SerializeStatus::FW_SERIALIZE_OK, status);
    buffer.resetSer();

    this->tlmWrite_U8Channel(u8chan);
    this->tlmWrite_U16Channel(u16chan);
    this->tlmWrite_U32Channel(u32chan);
    this->tlmWrite_I8Channel(i8chan);
    this->tlmWrite_I16Channel(i16chan);
    this->tlmWrite_I32Channel(i32chan);
    this->tlmWrite_StringEvenLength(evenString);
    this->tlmWrite_StringOddLength(oddString);
    this->tlmWrite_ArrayChannel(array);
    this->tlmWrite_StructChannel(structure);
    this->log_ACTIVITY_LO_ReceivedTelemetry(u8chan, u16chan, u32chan, i8chan, i16chan, i32chan, evenString, oddString,
                                            array, structure);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void TlmTestB::ECHO_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Fw::CmdStringArg& s) {
    this->log_ACTIVITY_LO_Echo(s);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace TlmTest
