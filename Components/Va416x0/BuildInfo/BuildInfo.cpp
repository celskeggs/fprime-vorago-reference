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
// \title  BuildInfo.cpp
// \brief  cpp file for BuildInfo component implementation class
// ======================================================================

#include "Components/Va416x0/BuildInfo/BuildInfo.hpp"
#include "Fw/Types/StringUtils.hpp"
#include "Types/StaticString.hpp"
#include "Va416x0/Svc/BuildInfo/BuildInfo.hpp"
#include "versions/version.hpp"

#include <algorithm>

namespace Va416x0 {

constexpr U8 EXP_LIBRARY_COUNT = 2;  // fprime-vorago, fprime-baremetal
// These indices correspond to the order of libraries in the `library_locations` field in the
// settings.ini configuration file
constexpr U8 FPRIME_VORAGO_VERSION_INDEX = 0;
constexpr U8 FPRIME_BAREMETAL_VERSION_INDEX = 1;

static inline FwSizeType strlen(const char* string) {
    return Fw::StringUtils::string_length(
        string, std::min(static_cast<U32>(FW_LOG_STRING_MAX_SIZE), static_cast<U32>(FW_TLM_STRING_MAX_SIZE)));
}

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

BuildInfo::BuildInfo(const char* const compName) : BuildInfoComponentBase(compName) {}

BuildInfo::~BuildInfo() {}

void BuildInfo::emitTelemetry() const {
    FW_ASSERT(Project::Version::LIBRARY_VERSIONS_COUNT == EXP_LIBRARY_COUNT, Project::Version::LIBRARY_VERSIONS_COUNT,
              EXP_LIBRARY_COUNT);
    // FIXME: future optimizations should autocode the length of build info/version strings to
    // avoid Fw::StringUtils::string_length calls on each invocation, this requires upstream
    // changes to fprime and fprime-vorago though

    // Emit deployment name telemetry
    Fw::StaticString deploymentName(Va416x0Svc::BuildInfo::DEPLOYMENT_NAME,
                                    strlen(Va416x0Svc::BuildInfo::DEPLOYMENT_NAME));
    this->log_ACTIVITY_LO_DeploymentName(deploymentName);
    this->tlmWrite_DeploymentName(deploymentName);

    // Emit build identifier telemetry
    Fw::StaticString buildIdentifier(Va416x0Svc::BuildInfo::BUILD_IDENTIFIER,
                                     strlen(Va416x0Svc::BuildInfo::BUILD_IDENTIFIER));
    this->log_ACTIVITY_LO_BuildIdentifier(buildIdentifier);
    this->tlmWrite_BuildIdentifier(buildIdentifier);

    // Emit project version telemetry
    Fw::StaticString projectVersion(Project::Version::PROJECT_VERSION, strlen(Project::Version::PROJECT_VERSION));
    this->log_ACTIVITY_LO_ProjectVersion(projectVersion);
    this->tlmWrite_ProjectVersion(projectVersion);

    // Emit fprime version telemetry
    Fw::StaticString fprimeVersion(Project::Version::FRAMEWORK_VERSION, strlen(Project::Version::FRAMEWORK_VERSION));
    this->log_ACTIVITY_LO_FprimeVersion(fprimeVersion);
    this->tlmWrite_FprimeVersion(fprimeVersion);

    // Emit fprime-vorago version telemetry
    Fw::StaticString fprimeVoragoVersion(Project::Version::LIBRARY_VERSIONS[FPRIME_VORAGO_VERSION_INDEX],
                                         strlen(Project::Version::LIBRARY_VERSIONS[FPRIME_VORAGO_VERSION_INDEX]));
    this->log_ACTIVITY_LO_FprimeVoragoVersion(fprimeVoragoVersion);
    this->tlmWrite_FprimeVoragoVersion(fprimeVoragoVersion);

    // Emit fprime-baremetal version telemetry
    Fw::StaticString fprimeBaremetalVersion(Project::Version::LIBRARY_VERSIONS[FPRIME_BAREMETAL_VERSION_INDEX],
                                            strlen(Project::Version::LIBRARY_VERSIONS[FPRIME_BAREMETAL_VERSION_INDEX]));
    this->log_ACTIVITY_LO_FprimeBaremetalVersion(fprimeBaremetalVersion);
    this->tlmWrite_FprimeBaremetalVersion(fprimeBaremetalVersion);

    // Report FW_DIRECT_PORT_CALLS
    // FIXME: Remove FwDirectPortCalls when no longer swapping between configurations
    this->log_ACTIVITY_LO_FwDirectPortCalls(FW_DIRECT_PORT_CALLS);
    this->tlmWrite_FwDirectPortCalls(FW_DIRECT_PORT_CALLS);
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void BuildInfo::BUILD_INFO_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // Emit build information and version events/telemetry in response to the command
    this->emitTelemetry();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Va416x0
