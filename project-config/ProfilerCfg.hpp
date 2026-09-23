// Copyright 2025 California Institute of Technology
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
//
// SPDX-License-Identifier: Apache-2.0

// ======================================================================
// \title  ProfilerCfg.hpp
// \brief  Configuration file for the Profiler class
// ======================================================================

#ifndef PROFILER_CFG_HPP
#define PROFILER_CFG_HPP

#include <Fw/FPrimeBasicTypes.hpp>

namespace Va416x0Svc {

//! NOTE: the profiler memory region is currently configured to use the latter 512 KiB of external
//! SRAM, this will need to shrink as other areas of software start to use external SRAM as well.
//! Also of note, this memory region cannot be used (i.e. the profiler cannot be enabled) until
//! external SRAM is configured on EBI.

//! Size of the memory region used to store profiler data, in bytes
constexpr U32 PROFILER_MEMORY_REGION_SIZE = 0x80000;  // 512 KiB

// Profiler external SRAM allocation
__attribute__((section(".EXTI_SRAM"), used)) U32 ProfilerExtSRAM[PROFILER_MEMORY_REGION_SIZE / sizeof(U32)];

//! Pointer to the start of the memory region used to store profiler data
constexpr U32* PROFILER_MEMORY_REGION_START = ProfilerExtSRAM;

}  // namespace Va416x0Svc

#endif
