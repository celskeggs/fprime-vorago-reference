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
// \title config/OsSelection.hpp
// \brief Vorago compile-time selection of Os::RawTime; Mutex/ConditionVariable keep the link-time delegates
//
// This header uses compile-time selection by aliasing Os::RawTime directly
// to Va416x0Os::TimerRawTime, bypassing the link-time DelegateRawTime
// mechanism used in F Prime's default configuration.
//
// Benefits of compile-time selection:
// - Eliminates virtual function dispatch overhead
// - Enables compiler to inline methods
// - Better optimization opportunities

// ======================================================================
#ifndef CONFIG_OSSELECTION_HPP
#define CONFIG_OSSELECTION_HPP

//!< Forward declaration of the concrete TimerRawTime implementation
namespace Va416x0Os {
class TimerSingleRawTime;
}
namespace Os {

using RawTime = Va416x0Os::TimerSingleRawTime;  //!< Compile-time alias to TimerRawTime (not DelegateRawTime)

//!< The override file replaces the whole default, so every selection must be
//!< present: keep Mutex and ConditionVariable on the link-time delegates
class DelegateMutex;
using Mutex = DelegateMutex;

class DelegateConditionVariable;
using ConditionVariable = DelegateConditionVariable;

}  // namespace Os

#define OS_RAW_TIME_HEADER "Va416x0/Os/TimerSingleRawTime/TimerSingleRawTime.hpp"
#define OS_MUTEX_HEADER <Os/DelegateMutex.hpp>
#define OS_CONDITION_VARIABLE_HEADER <Os/DelegateConditionVariable.hpp>

#endif  // CONFIG_OSSELECTION_HPP
