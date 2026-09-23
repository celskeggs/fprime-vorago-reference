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
// \title  TlmTestA.hpp
// \author root
// \brief  hpp file for TlmTestA component implementation class
// ======================================================================

#ifndef TlmTest_TlmTestA_HPP
#define TlmTest_TlmTestA_HPP

#include "Deployments/BreadboardGds/TlmTest/TlmTestA/TlmTestAComponentAc.hpp"

namespace TlmTest {

class TlmTestA final : public TlmTestAComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmTestA object
    TlmTestA(const char* const compName  //!< The component name
    );

    //! Destroy TlmTestA object
    ~TlmTestA();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    void run_handler(FwIndexType portNum, U32 context) override;
};

}  // namespace TlmTest

#endif
