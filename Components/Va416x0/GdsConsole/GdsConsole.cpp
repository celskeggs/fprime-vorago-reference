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
// \title  GdsConsole.cpp
// \brief  cpp file for GdsConsole
// ======================================================================

#include "Components/Va416x0/GdsConsole/GdsConsole.hpp"
#include "Os/Delegate.hpp"

namespace Va416x0 {

void GdsConsole::writeMessage(const CHAR* message, const FwSizeType size) {
    if (gdsStreamInstance != nullptr) {
        gdsStreamInstance->writeString(message, size);
    }
}

Os::ConsoleHandle* GdsConsole::getHandle() {
    return &this->m_handle;
}

}  // namespace Va416x0

namespace Os {

ConsoleInterface* ConsoleInterface::getDelegate(ConsoleHandleStorage& aligned_new_memory,
                                                const ConsoleInterface* to_copy) {
    return Os::Delegate::makeDelegate<ConsoleInterface, Va416x0::GdsConsole>(aligned_new_memory, to_copy);
}

}  // namespace Os

extern "C" {
int gds_console_put_char(char c) {
    int status = 0;
    if (Va416x0::gdsStreamInstance != nullptr) {
        status = Va416x0::gdsStreamInstance->writeChar(c);
    }
    return status;
}

int gds_console_flush(void) {
    int status = EOF;
    if (Va416x0::gdsStreamInstance != nullptr) {
        status = Va416x0::gdsStreamInstance->flushConsole();
    }
    return status;
}
}
