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

set(FPRIME_PLATFORM Linux)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_ASM_COMPILER gcc)

set(LINUX_COMMON_FLAGS "\
    -ggdb3 \
")

set(LINUX_COMMON_C_CXX_FLAGS "\
    -O3 \
    -fno-exceptions \
    -pedantic \
    -Wextra \
    -Wno-unused-parameter \
    -Werror \
")

set(CMAKE_C_FLAGS "\
    ${LINUX_COMMON_C_CXX_FLAGS} \
    -std=c17 \
")

set(CMAKE_CXX_FLAGS "\
    ${LINUX_COMMON_C_CXX_FLAGS} \
    -Wold-style-cast \
    -fno-rtti \
    -std=c++14 \
    -Wno-c++20-attribute-extensions \
")

set(CMAKE_ASM_FLAGS "\
    ${LINUX_COMMON_FLAGS} \
    -x assembler-with-cpp \
    -DDEBUG \
")

set(CMAKE_EXE_LINKER_FLAGS "\
    -Wl,--fatal-warnings \
    -Wl,-Map=linux.map \
")
