# Copyright 2026 California Institute of Technology
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

####
# This sets up the build system for the fprime-vorago-reference project, including
# components and deployments from project.cmake. In addition, it imports the core F Prime components.
####

cmake_minimum_required(VERSION 3.13)
project(fprime-vorago-reference C CXX)

# FIXME: fprime-vorago requires a CMAKE variable indicating whether
# the build is using direct-port-calls. Set the cpp macro here as well
# so that both are set in one location
# This is temporary and will be removed (and FpConfig.h used to set the macro)
# once https://github.com/fprime-community/fprime-vorago/issues/15 is implemented
if (BUILD_TESTING)
    add_compile_options(
      -DFW_DIRECT_PORT_CALLS=0
    )
    set(CMAKE_FW_DIRECT_PORT_CALLS 0)
else()
    add_compile_options(
      -DFW_DIRECT_PORT_CALLS=1
    )
    set(CMAKE_FW_DIRECT_PORT_CALLS 1)
endif()

# Skipping the version check because we're using a branch of FPP that's
# not tagged for release
# FIXME: See discussion in https://github.com/nasa/fprime/issues/4567 for
# discussion of a new feature to support custom/flexible fpp versions
set(FPRIME_SKIP_TOOLS_VERSION_CHECK 1)

###
# F' Core Setup
# This includes all of the F prime core components, and imports the make-system.
###
include("${CMAKE_CURRENT_LIST_DIR}/lib/fprime/cmake/FPrime.cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# When compiling the fprime-baremetal library, include overriding
# the new & delete operators with version that use a MemAllocator
set(FPRIME_BAREMENTAL_OVERRIDE_NEW_DELETE TRUE)

# Ensure that we do not set -DNDEBUG in release builds
# (We only use release builds, and we WANT assertions.)
set(CMAKE_CXX_FLAGS_RELEASE "-Os ")
register_fprime_build_autocoder("${CMAKE_CURRENT_LIST_DIR}/lib/fprime-vorago/cmake/autocoders/python_autocoder.cmake" OFF)
register_fprime_target("${CMAKE_CURRENT_LIST_DIR}/cmake/autocoders/telemetry_autocoder.cmake")

fprime_setup_included_code()

# WORKAROUND Remove once https://github.com/fprime-community/fprime-vorago/pull/75 is merged
if (TARGET Fw_Types AND TARGET project_config)
    add_dependencies(Fw_Types project_config)
endif()
# WORKAROUND: Add fprime framework path so BuildInfoAc can find Fw/Types/BasicTypes.hpp
# (referenced by FppConstantsAc.hpp). 
list(APPEND FPRIME_BUILD_LOCATIONS "${FPRIME_FRAMEWORK_PATH}")

# Exclude components which assert that FwPacketDescriptor conforms to the Space Packet Protocol.
# This project does not conform to SPP and does not make use of these components.
set_target_properties(Svc_Ccsds_ApidManager Svc_FprimeDeframer Svc_Ccsds_TmFramer Svc_Ccsds_AosFramer Svc_Ccsds_AosDeframer PROPERTIES
    EXCLUDE_FROM_ALL TRUE
)

# Exclude Svc_FpySequencer: it statically asserts FW_PORT_SERIALIZATION == 1, but this project builds
# with FW_DIRECT_PORT_CALLS=1 (port serialization disabled). No component or deployment here uses it.
set_target_properties(Svc_FpySequencer PROPERTIES
    EXCLUDE_FROM_ALL TRUE
)

# Exclude Svc_Ccsds_TestUtils: We're using an 8-bit APID (rather than 11-bit as dictated by CCSDS)
# so there's a type-limit warning, see https://github.com/nasa/fprime/pull/5534
if (BUILD_TESTING)
    set_target_properties(Svc_Ccsds_TestUtils PROPERTIES
        EXCLUDE_FROM_ALL TRUE
    )
endif()

# Include project-specific cmake functions here (must be called before project.cmake include to
# make the functions available to subdirectory CMakeLists.txt files)
# Define external memory regions and build them into dictionaries
include("${CMAKE_CURRENT_LIST_DIR}/cmake/external_memory.cmake")

# This includes project-wide objects
include("${CMAKE_CURRENT_LIST_DIR}/project.cmake")
