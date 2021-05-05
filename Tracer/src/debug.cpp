/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <limits>

#include "debug.h"

namespace rt {

  namespace debug {

    constexpr size_t INVALID = std::numeric_limits<size_t>::max();

    constexpr size_t TRIGGER_X = 10;
    constexpr size_t TRIGGER_Y = 10;

    size_t trigger_x = INVALID;
    size_t trigger_y = INVALID;

    volatile size_t tick{0};

  } // namespace debug

  void enableDebug(const size_t x, const size_t y)
  {
    using namespace debug;
    if( x == TRIGGER_X  &&  y == TRIGGER_Y ) {
      trigger_x = TRIGGER_X;
      trigger_y = TRIGGER_Y;
    }
  }

  void disableDebug()
  {
    using namespace debug;
    if( isDebug() ) {
      trigger_x = trigger_y = INVALID;
    }
  }

  bool isDebug()
  {
    using namespace debug;
    return trigger_x == TRIGGER_X  &&  trigger_y == TRIGGER_Y;
  }

  void idleDebug()
  {
    if( !isDebug() ) {
      return;
    }
    debug::tick += 1;
  }

  void debugMessage(const char *msg, const bool nl)
  {
    if( !isDebug() ) {
      return;
    }
    printf("%s%s", msg, nl ? "\n" : "");
    fflush(stdout);
  }

} // namespace rt
