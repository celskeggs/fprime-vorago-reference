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
// \title  MemoryAllocation.hpp
// \brief  hpp file for types used for ADC interactions
// ======================================================================

#ifndef Types_MemoryAllocation_HPP
#define Types_MemoryAllocation_HPP
namespace MemoryAllocation {

enum {
    MEM_ID_UNKNOWN = 0,
    MEM_ID_DEPLOYMENT_SETUP,
    MEM_ID_GDS_SUPPORT_TLM_SEND,
    MEM_ID_GDS_SUPPORT_CMD_DISP,
    MEM_ID_PLACEHOLDER1,
    MEM_ID_PLACEHOLDER2,
    MEM_ID_PLACEHOLDER3,
    MEM_ID_NUM_IDS
};
}
#endif  // Types_MemoryAllocation_HPP
