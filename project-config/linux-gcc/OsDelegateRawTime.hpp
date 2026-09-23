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
// \brief Linux/test override: alias Os::RawTime to the link-time delegate
// ======================================================================
#ifndef CONFIG_OS_DELEGATERAWTIME_HPP
#define CONFIG_OS_DELEGATERAWTIME_HPP

namespace Os {

class DelegateRawTime;
using RawTime = DelegateRawTime;

}  // namespace Os

#define OS_RAW_TIME_HEADER <Os/DelegateRawTime.hpp>

#endif  // CONFIG_OS_DELEGATERAWTIME_HPP
