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
// \title  BuildInfo.hpp
// \brief  hpp file for BuildInfo component implementation class
// ======================================================================

#ifndef Va416x0_BuildInfo_HPP
#define Va416x0_BuildInfo_HPP

#include "Components/Va416x0/BuildInfo/BuildInfoComponentAc.hpp"

namespace Va416x0 {

class BuildInfo final : public BuildInfoComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct BuildInfo object
    BuildInfo(const char* const compName  //!< The component name
    );

    //! Destroy BuildInfo object
    ~BuildInfo();

    //! Emit all build information and version events and telemetry
    void emitTelemetry() const;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command BUILD_INFO
    //!
    //! Report the requested build information via events and telemetry
    void BUILD_INFO_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                               U32 cmdSeq            //!< The command sequence number
                               ) override;
};

}  // namespace Va416x0

#endif
