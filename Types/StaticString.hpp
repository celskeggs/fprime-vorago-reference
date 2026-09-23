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
// @file   StaticString.hpp
// @author Ian Brault
// @brief  A string backed by an immutable string literal
// ======================================================================

// FIXME: This is a temporary solution and should be removed once fpp supports the underlying
// feature (blocked by https://github.com/nasa/fpp/issues/869 fix)

#ifndef FW_STATIC_STRING_HPP
#define FW_STATIC_STRING_HPP

#include <Fw/FPrimeBasicTypes.hpp>
#include <Fw/Types/StringBase.hpp>

namespace Fw {

//! A string backed by an immutable string literal
class StaticString final : public Fw::StringBase {
  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Constructor (uninitialized buffer)
    StaticString() : StringBase(), m_bufferPtr(nullptr), m_bufferSize(0) {}

    //! Constructor (bufferPtr and bufferSize)
    StaticString(const char* bufferPtr,           //!< The buffer pointer
                 StringBase::SizeType bufferSize  //!< The buffer size
                 )
        : StringBase(), m_bufferPtr(bufferPtr), m_bufferSize(bufferSize + 1) {}

    //! Destructor
    ~StaticString() {}

  public:
    // ----------------------------------------------------------------------
    // StringBase interface
    // ----------------------------------------------------------------------

    //! Gets the char buffer
    const char* toChar() const { return this->m_bufferPtr; }

    //! Returns the buffer size
    StringBase::SizeType getCapacity() const { return this->m_bufferSize; }

  private:
    // ----------------------------------------------------------------------
    // Data members
    // ----------------------------------------------------------------------

    //! Pointer to string buffer
    const char* m_bufferPtr;
    //! Size of string buffer
    //! F Prime strings are null-terminated, so this is one more than
    //! the length of the largest string that the buffer can hold
    StringBase::SizeType m_bufferSize;
};

}  // namespace Fw

#endif
