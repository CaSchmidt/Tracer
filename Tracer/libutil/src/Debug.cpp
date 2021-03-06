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

#include "Debug.h"

namespace priv_debug {

  constexpr n4::size_t INVALID = std::numeric_limits<n4::size_t>::max();

  constexpr n4::size_t TRIGGER_X = 10;
  constexpr n4::size_t TRIGGER_Y = 10;

  n4::size_t trigger_x = INVALID;
  n4::size_t trigger_y = INVALID;

  volatile n4::size_t tick{0};

} // namespace priv_debug

void enableDebug(const n4::size_t x, const n4::size_t y)
{
  using namespace priv_debug;
  if( x == TRIGGER_X  &&  y == TRIGGER_Y ) {
    trigger_x = TRIGGER_X;
    trigger_y = TRIGGER_Y;
  }
}

void disableDebug()
{
  using namespace priv_debug;
  trigger_x = trigger_y = INVALID;
}

bool isDebug()
{
  using namespace priv_debug;
  return trigger_x == TRIGGER_X  &&  trigger_y == TRIGGER_Y;
}

void idleDebug()
{
  if( !isDebug() ) {
    return;
  }
  priv_debug::tick += 1;
}

void debugMessage(const char *msg, const bool nl)
{
  if( !isDebug() ) {
    return;
  }
  printf("%s%s", msg, nl ? "\n" : "");
  fflush(stdout);
}
