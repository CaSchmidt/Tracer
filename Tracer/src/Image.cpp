/****************************************************************************
** Copyright (c) 2019, Carsten Schmidt. All rights reserved.
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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <3rdparty/stb/include/stb_image_write.h>

#include "Image.h"

////// Macros ////////////////////////////////////////////////////////////////

#define STBI_COMP_RGBA8888  4

////// public ////////////////////////////////////////////////////////////////

Image::Image(const int width, const int height,
             const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept
{
  if( width < 1  ||  height < 1 ) {
    return;
  }

  _width  = width;
  _height = height;

  try {
    _buffer.resize(static_cast<size_type>(stride()*_height), 0xFF);
  } catch(...) {
    _width = _height = 0;
    _buffer.clear();
    return;
  }

  fill(r, g, b, a);
}

bool Image::isEmpty() const
{
  return _buffer.empty();
}

uint8_t *Image::row(const int y) const
{
  if( isEmpty()  ||  y < 0  ||  y >= _height ) {
    return nullptr;
  }
  return const_cast<uint8_t*>(_buffer.data() + static_cast<size_type>(stride()*y));
}

void Image::fill(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
  for(int y = 0; y < _height; y++) {
    uint8_t *data = row(y);
    for(int x = 0; x < _width; x++) {
      *data++ = r;
      *data++ = g;
      *data++ = b;
      *data++ = a;
    }
  }
}

int Image::stride() const
{
  return _width*4;
}

int Image::width() const
{
  return _width;
}

int Image::height() const
{
  return _height;
}

bool Image::saveAsPNG(const char *filename) const
{
  if( isEmpty() ) {
    return false;
  }
  return stbi_write_png(filename, width(), height(), STBI_COMP_RGBA8888, row(0), stride()) != 0;
}
