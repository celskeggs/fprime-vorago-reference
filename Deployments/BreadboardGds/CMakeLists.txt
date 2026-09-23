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

###
# Topology and Components
###

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/TlmTest/")

register_fprime_deployment(
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/BreadboardGds.cpp"
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/BreadboardGds.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/../DeploymentConstants.fpp"
    DEPENDS
        Va416x0_Svc_MainLoop
        Va416x0_Svc_Metronome
        fprime-baremetal_Svc_TlmLinearChan
        Va416x0_Svc_VectorTable
        Va416x0_Svc_StrictMallocAllocator
        Components_Va416x0_GdsSupport
        Deployments_Constants
)

# Call after register_fprime_deployment so PROJECT_NAME exists.
register_with_bsp("${FPRIME_CURRENT_MODULE}")

# Needed to support Va416x0.GdsSupport without external SRAM
link_breadboard_extsram("${FPRIME_CURRENT_MODULE}")
link_external_memory("${FPRIME_CURRENT_MODULE}")
