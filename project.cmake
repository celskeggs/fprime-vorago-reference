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

# This CMake file is intended to register project-wide objects.
# This allows for reuse between deployments, or other projects.

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/project-config/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Ports/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Components/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Deployments/")
