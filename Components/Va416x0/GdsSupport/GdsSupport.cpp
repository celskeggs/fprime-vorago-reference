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
// \title  GdsSupport.cpp
// \brief  cpp file for GdsSupport subtopology
// ======================================================================

#include "Components/Va416x0/GdsSupport/GdsSupport.hpp"
#include "Components/Va416x0/GdsConsole/GdsConsole.hpp"
#include "Fw/Types/MallocAllocator.hpp"
#include "Types/MemoryAllocation.hpp"
#include "Va416x0/Os/TimerRawTime/TimerRawTime.hpp"

namespace Va416x0 {

namespace GdsSupport {

void preconfigure(Fw::MemAllocator& mallocator) {
    // Allocate memory needed by components
    tlmSend.setup(MemoryAllocation::MEM_ID_GDS_SUPPORT_TLM_SEND, mallocator);
    cmdDisp.setup(MemoryAllocation::MEM_ID_GDS_SUPPORT_CMD_DISP, mallocator);
    // And perform other pre-initialize setup
    gdsStream.setup();
}

void initialize(Fw::MemAllocator& mallocator) {
    // Configure rate group component with zero-initialized contexts
    // Use single-element constructor to fill all elements with 0
    Svc::PassiveRateGroup::ContextArray contexts(0);

    // This static_assert ensures MainLoop's default raw_time_source matches our configure() call.
    // If fprime-vorago changes MainLoop's default, this will fail at compile time.
    static_assert(std::is_same<Os::RawTime, Va416x0Os::TimerSingleRawTime>::value,
                  "Os::RawTime must alias Va416x0Os::TimerSingleRawTime to match MainLoop's default");

    // Notice: the timer source must match between MainLoop and
    // PassiveRateGroup configuration.
    // Parameter raw_time_source in MainLoop::configure function declaration
    // has a default value of Os::RAWTIME_TIMER_SINGLE.
    rateGroup.configure(contexts, Os::RAWTIME_TIMER_SINGLE);
}

}  // namespace GdsSupport

}  // namespace Va416x0

Va416x0Svc::VectorTable* const va416x0_vector_table_instance = &Va416x0::GdsSupport::vectorTable;
Va416x0::GdsStream* Va416x0::gdsStreamInstance = &Va416x0::GdsSupport::gdsStream;
