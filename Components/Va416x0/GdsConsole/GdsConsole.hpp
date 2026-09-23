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
// \title  GdsConsole.hpp
// \brief  hpp file for GdsConsole
// ======================================================================

#ifndef Va416x0_GdsConsole_HPP
#define Va416x0_GdsConsole_HPP

#include "Va416x0/Svc/GdsStream/GdsStream.hpp"
#include "Os/Console.hpp"

extern "C" {
#include "stdio.h"
}

namespace Va416x0 {

struct GdsConsoleHandle : public Os::ConsoleHandle {};

class GdsConsole : public Os::ConsoleInterface {
  public:
    //! GdsConsole constructor (default)
    GdsConsole() = default;
    //! Copy constructor (default)
    GdsConsole(const GdsConsole& other) = default;
    //! Assignment operator (default)
    GdsConsole& operator=(const GdsConsole& other) = default;
    //! GdsConsole constructor (default)
    ~GdsConsole() override = default;

    // ------------------------------------
    // Os::ConsoleInterface overrides
    // ------------------------------------

    //! Write a message to console
    void writeMessage(const CHAR* message, const FwSizeType size) override;

    //! Return the raw console handle
    Os::ConsoleHandle* getHandle() override;

  private:
    GdsConsoleHandle m_handle;
};

//! Pointer to a global singleton GdsStream instance, this is a raw pointer instead of a reference
//! so that deployments without a GdsStream can set this to nullptr
extern GdsStream* gdsStreamInstance;

}  // namespace Va416x0

#endif
