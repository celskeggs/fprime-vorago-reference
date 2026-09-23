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
// \title config/OsDelegateRawTime.hpp
// \brief Vorago compile-time selection of Os::RawTime implementation
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
#ifndef CONFIG_OS_DELEGATERAWTIME_HPP
#define CONFIG_OS_DELEGATERAWTIME_HPP

//!< Forward declaration of the concrete TimerRawTime implementation
namespace Va416x0Os {
class TimerSingleRawTime;
}
namespace Os {

using RawTime = Va416x0Os::TimerSingleRawTime;  //!< Compile-time alias to TimerRawTime (not DelegateRawTime)

}  // namespace Os

#define OS_RAW_TIME_HEADER "Va416x0/Os/TimerSingleRawTime/TimerSingleRawTime.hpp"

#endif  // CONFIG_OS_DELEGATERAWTIME_HPP
