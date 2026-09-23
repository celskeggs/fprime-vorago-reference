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
# project-config/CMakeLists.txt:
#
# Sets a list of source files for cmake to process as part of autocoding.
####

set(CONSOLE_IMPLEMENTATION Components_Va416x0_GdsConsole)
# GdsConsole is not available for Linux platform builds so stub out the console implementation
if ((BUILD_TESTING) OR (FPRIME_PLATFORM STREQUAL "Linux"))
    set(CONSOLE_IMPLEMENTATION Os_Console_Posix)
endif ()

if ((BUILD_TESTING) OR (FPRIME_PLATFORM STREQUAL "Linux"))
    set(OS_RAWTIME_CFG "${CMAKE_CURRENT_LIST_DIR}/linux-gcc/OsDelegateRawTime.hpp")
    set(USE_VORAGO_RAWTIME FALSE)
else()
    set(OS_RAWTIME_CFG "${CMAKE_CURRENT_LIST_DIR}/vorago/OsDelegateRawTime.hpp")
    set(USE_VORAGO_RAWTIME TRUE)
endif ()

register_fprime_config(
        project-config
    CONFIGURATION_OVERRIDES
        "${OS_RAWTIME_CFG}"
        "${CMAKE_CURRENT_LIST_DIR}/AcConstants.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/ComCfg.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/CommandDispatcherImplCfg.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/FpConfig.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/FpConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/FpConstants.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/PlatformCfg.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/ProfilerCfg.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/TlmChanImplCfg.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/PrmDbImplCfg.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/MemoryAllocation.fpp"
    INTERFACE
    CHOOSES_IMPLEMENTATIONS
        "${CONSOLE_IMPLEMENTATION}"
)

if (USE_VORAGO_RAWTIME)
    # vorago/OsDelegateRawTime.hpp (selected above) compile-time aliases Os::RawTime to
    # Va416x0Os::TimerSingleRawTime, so any source that includes Os/RawTime.hpp transitively pulls in
    # TimerSingleRawTime.hpp and the autocoded headers of its dependencies (e.g. Va416x0_Mmio_Timer/
    # ClkTree/Types). Include *resolution* is never the problem (fprime puts the whole tree on -I);
    # the problem is purely build *ordering*: those Va416x0 autocoders must run before any fprime
    # Os/Svc source that includes RawTime.hpp is compiled.
    #
    # There is no single anchor target to attach this to: RawTime.hpp is included by targets that do
    # NOT link any RawTime/Os target (e.g. Os_Models DEPENDS only Fw_Types, the Svc_Ports_OsTimeEpoch
    # port autocode, Svc_TlmChan) and by Os_RawTime implementors (which cannot link the interface they
    # implement, e.g. Svc_OsTime_test_RawTimeTester). So the dependency must be added directly to each
    # fprime Os/Svc module. Module targets are named after their directory path relative to the build
    # root (Os/Queue -> Os_Queue), so "Os"/"Os_*" and "Svc"/"Svc_*" cover every such target.
    #
    # This must be a link edge (fprime_target_dependencies), not add_dependencies: CMake derives each
    # target's cmake_object_order_depends ordering (which gates compiling that target's own sources,
    # e.g. an #include of an autocoded header) from the link-library closure at generate time, so
    # add_dependencies would only order final link/archive steps and would NOT prevent #include races.
    get_property(ALL_FPRIME_MODULES GLOBAL PROPERTY FPRIME_MODULES)
    foreach (FPRIME_MODULE_NAME IN LISTS ALL_FPRIME_MODULES)
        if (FPRIME_MODULE_NAME MATCHES "^(Os|Svc)(_.+)?$" AND TARGET "${FPRIME_MODULE_NAME}")
            fprime_target_dependencies("${FPRIME_MODULE_NAME}" PUBLIC Va416x0_Os_TimerSingleRawTime)
        endif()
    endforeach()
endif()
