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
// \brief Linux/test override: keep the link-time delegate selections
// ======================================================================
#ifndef CONFIG_OSSELECTION_HPP
#define CONFIG_OSSELECTION_HPP

namespace Os {

class DelegateRawTime;
using RawTime = DelegateRawTime;

class DelegateMutex;
using Mutex = DelegateMutex;

class DelegateConditionVariable;
using ConditionVariable = DelegateConditionVariable;

}  // namespace Os

#define OS_RAW_TIME_HEADER <Os/DelegateRawTime.hpp>
#define OS_MUTEX_HEADER <Os/DelegateMutex.hpp>
#define OS_CONDITION_VARIABLE_HEADER <Os/DelegateConditionVariable.hpp>

#endif  // CONFIG_OSSELECTION_HPP
