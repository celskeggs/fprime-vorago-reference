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
// \title  GdsSupport.hpp
// \brief  GdsSupport initialization helpers
// ======================================================================

#ifndef Va416x0_GdsSupport_HPP
#define Va416x0_GdsSupport_HPP

#include "Va416x0/Svc/GdsStream/GdsStream.hpp"
#include "Va416x0/Svc/TlmGdsChan/TlmGdsChan.hpp"
#include "Fw/Types/MallocAllocator.hpp"
#include "Svc/PassiveRateGroup/PassiveRateGroup.hpp"
#include "Va416x0/Svc/Metronome/Metronome.hpp"
#include "Va416x0/Svc/VectorTable/VectorTable.hpp"
#include "fprime-baremetal/Svc/PassiveCmdDispatcher/PassiveCmdDispatcher.hpp"

namespace Va416x0 {

namespace GdsSupport {

// Declared here but must be defined in deployment hand-code
extern Va416x0Svc::MetronomeConfig metronome_config;

// Declared here but must be defined in deployment autocode
extern Baremetal::PassiveCmdDispatcher cmdDisp;
extern Va416x0::GdsStream gdsStream;
extern Va416x0::TlmGdsChan tlmSend;
extern Svc::PassiveRateGroup rateGroup;
extern Va416x0Svc::VectorTable vectorTable;

void preconfigure(Fw::MemAllocator& mallocator);
//! initialize (since GdsSupport allocates several large chunks of memory, it uses multiple memory IDs)
void initialize(Fw::MemAllocator& mallocator);

}  // namespace GdsSupport

}  // namespace Va416x0

#endif
