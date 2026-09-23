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


# All deployments that use the Metronome (for scheduling primary rate group)
# should have a Constants.fpp and register it here
register_fprime_module(
    Deployments_Constants
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/DeploymentConstants.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/BreadboardGds/Constants.fpp"
)

# Add deployments here
if (FPRIME_PLATFORM STREQUAL "va416x0-baremetal")
    # Exclude breadboard deployments (which do not support external SRAM) from profiler builds
    if ((NOT DEFINED VA416X0_ENABLE_PROFILER) OR (NOT VA416X0_ENABLE_PROFILER))
        add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/BreadboardGds/")
    endif ()
endif ()

