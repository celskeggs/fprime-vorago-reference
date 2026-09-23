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
// \title  TlmTestA.cpp
// \author root
// \brief  cpp file for TlmTestA component implementation class
// ======================================================================

#include "Deployments/BreadboardGds/TlmTest/TlmTestA/TlmTestA.hpp"

namespace TlmTest {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmTestA::TlmTestA(const char* const compName) : TlmTestAComponentBase(compName) {}

TlmTestA::~TlmTestA() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void TlmTestA::run_handler(FwIndexType portNum, U32 context) {
    Fw::String evenString("foobar");
    Fw::String oddString("bazbazbaz");
    TlmTest::U8x3 array = {1, 2, 3};
    TlmTest::StructChan structure = {1, 123, 123456};

    this->tlmWrite_U8Channel(0xA5);
    this->tlmWrite_U16Channel(0xABCD);
    this->tlmWrite_U32Channel(0x12345678);
    this->tlmWrite_I8Channel(-3);
    this->tlmWrite_I16Channel(-123);
    this->tlmWrite_I32Channel(-888888);
    this->tlmWrite_StringEvenLength(evenString);
    this->tlmWrite_StringOddLength(oddString);
    this->tlmWrite_ArrayChannel(array);
    this->tlmWrite_StructChannel(structure);
}

}  // namespace TlmTest
