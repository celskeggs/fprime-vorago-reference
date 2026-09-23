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
# FPrime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
# UT_SOURCE_FILES: list of source files for unit tests
#
# More information in the F´ CMake API documentation:
# https://fprime.jpl.nasa.gov/latest/docs/user-manual/build-system/cmake-api/
#
####

set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/BuildInfo.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/BuildInfo.cpp"
)

# Uncomment and add any modules that this component depends on, else
# they might not be available when cmake tries to build this component.
set(MOD_DEPS
   version
)

register_fprime_module()


### Unit Tests ###
# set(UT_SOURCE_FILES
#   "${CMAKE_CURRENT_LIST_DIR}/BuildInfo.fpp"
#   "${CMAKE_CURRENT_LIST_DIR}/test/ut/BuildInfoTestMain.cpp"
#   "${CMAKE_CURRENT_LIST_DIR}/test/ut/BuildInfoTester.cpp"
# )
# set(UT_MOD_DEPS
#   STest
# )
# set(UT_AUTO_HELPERS ON)
# register_fprime_ut()
