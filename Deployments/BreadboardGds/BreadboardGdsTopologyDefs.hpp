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
// \title  BreadboardGdsTopologyDefs.hpp
// \brief required header file containing the required definitions for the topology autocoder
//
// ======================================================================

#ifndef BREADBOARD_BreadboardGdsTOPOLOGYDEFS_HPP
#define BREADBOARD_BreadboardGdsTOPOLOGYDEFS_HPP

#include "Components/Va416x0/GdsSupport/GdsSupport.hpp"
#include "Fw/Types/BasicTypes.hpp"
#include "Va416x0/Mmio/ClkTree/ClkTree.hpp"
#include "Va416x0/Svc/Metronome/Metronome.hpp"
#include "Va416x0/Svc/StrictMallocAllocator/StrictMallocAllocator.hpp"

// Definitions are placed within a namespace named after the deployment
namespace BreadboardGds {
extern Va416x0Svc::StrictMallocAllocator mallocator;
struct TopologyState {};

}  // namespace BreadboardGds

#endif
