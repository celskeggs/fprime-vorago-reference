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

#include <Deployments/BreadboardGds/BreadboardGdsTopologyAc.hpp>
#include <Fw/Types/MallocAllocator.hpp>
#include <Os/RawTime.hpp>
#include <type_traits>
#include "BreadboardGdsTopologyDefs.hpp"
#include "Deployments/FppConstantsAc.hpp"
#include "Types/MemoryAllocation.hpp"
#include "Va416x0/Mmio/Nvic/Nvic.hpp"
#include "Va416x0/Mmio/SysTick/SysTick.hpp"
#include "Va416x0/Os/TimerRawTime/TimerRawTime.hpp"

BreadboardGds::TopologyState state;
Va416x0Svc::StrictMallocAllocator BreadboardGds::mallocator;

void initialize_deployment() {
    BreadboardGds::mallocator.setup(MemoryAllocation::MEM_ID_NUM_IDS, MemoryAllocation::MEM_ID_DEPLOYMENT_SETUP);
    Fw::MemAllocatorRegistry& registry = Fw::MemAllocatorRegistry::getInstance();
    registry.registerAllocator(Fw::MemoryAllocation::MemoryAllocatorType::SYSTEM, BreadboardGds::mallocator);

    Va416x0::GdsSupport::preconfigure(BreadboardGds::mallocator);
    BreadboardGds::setup(state);
    BreadboardGds::mallocator.setDefaultId(MemoryAllocation::MEM_ID_UNKNOWN);
    Va416x0::GdsSupport::mainLoop.configure(Va416x0Mmio::ClkTree::createClockTree(
        // 80 MHz
        80000000,
        // Not using oscillator or PLL
        0, 0, 0, 0, 0,
        // Use external clock without division
        1, Va416x0Mmio::SysclkSource::EXTERNAL_CLK,
        // No PLL
        Va416x0Mmio::PllSource::NONE,
        // Run ADC at SystemClk/8 (10 MHz)
        8));
    Va416x0Os::TimerRawTime::configure(1, 2, Va416x0Os::TimerRawTime::TIMER_BITS64);
    Va416x0Os::TimerRawTime::initPeripherals();
    Va416x0Os::TimerSingleRawTime::configure(4);  // To enable CPU-efficient delta-time operations
    Va416x0::GdsSupport::sclk.configure(Fw::Time(TimeBase::TB_PROC_TIME, 1000, 0),
                                        Va416x0::GdsSupport::metronome_config.default_duration_micros, 64);
    Va416x0::GdsSupport::initialize(BreadboardGds::mallocator);
    BreadboardGds::buildInfo.emitTelemetry();

    BreadboardGds::mallocator.disableAllocation();
    BreadboardGds::mallocator.reportAllocation();
}

Va416x0Svc::MetronomeConfig Va416x0::GdsSupport::metronome_config = {
    // Timers 14 and 15 assigned to RTI handling
    Va416x0Mmio::Timer(14),
    Va416x0Mmio::Timer(15),
    BreadboardGds::RtiMinDurationMicros,
    BreadboardGds::RtiDefaultDurationMicros,
    BreadboardGds::RtiMaxDurationMicros,
    {
        /* No clients */
    },
    Va416x0Mmio::Nvic::PRIORITY_GROUP_4,
    Va416x0Mmio::Nvic::PRIORITY_GROUP_4,
};
