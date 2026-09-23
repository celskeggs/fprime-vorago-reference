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
// \title  TlmTestB.hpp
// \author root
// \brief  hpp file for TlmTestB component implementation class
// ======================================================================

#ifndef TlmTest_TlmTestB_HPP
#define TlmTest_TlmTestB_HPP

#include "Deployments/BreadboardGds/TlmTest/TlmTestB/TlmTestBComponentAc.hpp"

namespace TlmTest {

class TlmTestB final : public TlmTestBComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmTestB object
    TlmTestB(const char* const compName  //!< The component name
    );

    //! Destroy TlmTestB object
    ~TlmTestB();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command TRIGGER
    void TRIGGER_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                            U32 cmdSeq            //!< The command sequence number
                            ) override;

    //! Handler implementation for command ECHO
    void ECHO_cmdHandler(FwOpcodeType opCode,       //!< The opcode
                         U32 cmdSeq,                //!< The command sequence number
                         const Fw::CmdStringArg& s  //!< Command argument
                         ) override;
};

}  // namespace TlmTest

#endif
