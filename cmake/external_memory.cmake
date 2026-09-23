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

# NOTE: list supported external memory sections here
# These names should exactly match the sections from the "SECTIONS" portion of the linker script
set(EXTMEM_SECTIONS
    ".EXTI_SRAM"
)

set(EXTMEM_LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/toolchain/external_memory.ld")
set(EXTMEM_PYTHON_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/scripts/build_extmem_dictionaries.py")

# External memory regions are defined in a linker script. Deployments and components can use
# these regions to instruct the linker to allocate chunks of memory within the respective address
# spaces. This CMake function allows deployments to include these defined regions for linking and
# adds a post-build target to feed any external memory allocations made by the linker back into the
# dictionaries.
# NOTE: this must be called after merge_dictionaries because it expects the merged dictionary as
# the input dictionary
function(link_external_memory TARGET_NAME)
    # Get the deployment name from the current directory
    # This is different from ${TARGET_NAME} which includes a "Deployments_" prefix
    get_filename_component(
        DEPLOYMENT
        ${CMAKE_CURRENT_LIST_DIR}
        NAME
    )
    set(BUILD_PATH "${CMAKE_BINARY_DIR}/Deployments/${DEPLOYMENT}")

    # Use paths in the build directory, not install directory, to avoid circular dependencies
    set(ELF_PATH "${CMAKE_BINARY_DIR}/bin/${TOOLCHAIN_NAME}/${TARGET_NAME}.elf")
    set(OUTPUT_DICT_PATH "${BUILD_PATH}/${DEPLOYMENT}TopologyDictionary-extmem.json")
    # If the deployment has a merged dictionary, use the merged dictionary as input,
    # otherwise use the standard topology dictionary (both from build directory)
    if (EXISTS "${BUILD_PATH}/${DEPLOYMENT}TopologyDictionary-merged.json")
        set(DICT_PATH "${BUILD_PATH}/${DEPLOYMENT}TopologyDictionary-merged.json")
    else ()
        set(DICT_PATH "${BUILD_PATH}/${DEPLOYMENT}TopologyDictionary.json")
    endif ()

    # Add the external memory linker script to the target
    target_link_options("${TARGET_NAME}" PRIVATE "-Wl,-T${EXTMEM_LINKER_SCRIPT}")

    # Add the post-build command to feed linker allocations of external memory back into the
    # deployment dictionary, then install it following fprime's dictionary installation
    # pattern
    # Note: DEPENDS removed - POST_BUILD already ensures ELF and dict exist, and DEPENDS
    # can cause Ninja rebuild loops when combined with install commands
    set(ACTUAL_INSTALL_DIR "${FPRIME_INSTALL_DEST}/${TOOLCHAIN_NAME}/${TARGET_NAME}/dict/extmem")
    add_custom_command(
        TARGET "${TARGET_NAME}" POST_BUILD
        COMMAND "${PYTHON}" "${EXTMEM_PYTHON_SCRIPT}" "${ELF_PATH}" "${DICT_PATH}" "${OUTPUT_DICT_PATH}" "${EXTMEM_SECTIONS}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${ACTUAL_INSTALL_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${OUTPUT_DICT_PATH}" "${ACTUAL_INSTALL_DIR}/${DEPLOYMENT}TopologyDictionary-extmem.json"
        COMMENT "Parse linker allocations of external memory regions and install extmem dictionary"
        VERBATIM
    )
endfunction()

set(BREADBOARD_EXTSRAM_LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/toolchain/breadboard_gds_memory.ld")

# This CMake function adds support for external SRAM for breadboard deployments which do not have
# actual external SRAM by adding the breadboard_gds_memory.ld linker script to the target which
# moves the .EXTI_SRAM section inside the DATA_SRAM0 memory region
# NOTE: this should be called before link_external_memory is called
function(link_breadboard_extsram TARGET_NAME)
    # Add the breadboard linker script to the target
    target_link_options("${TARGET_NAME}" PRIVATE "-Wl,-T${BREADBOARD_EXTSRAM_LINKER_SCRIPT}")
endfunction()
