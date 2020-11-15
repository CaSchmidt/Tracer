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

#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Image.h"

////// Macros ////////////////////////////////////////////////////////////////

#define STBI_COMP_RGBA8888  4

////// public ////////////////////////////////////////////////////////////////

Image::Image(const size_type width, const size_type height,
             const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept
{
  resize(width, height, r, g, b, a);
}

bool Image::isEmpty() const
{
  return _buffer.empty();
}

bool Image::isValidX(const size_type x) const
{
  return !isEmpty()  &&  x >= 0  &&  x < width();
}

bool Image::isValidY(const size_type y) const
{
  return !isEmpty()  &&  y >= 0  &&  y < height();
}

void Image::clear()
{
  _width = _height = 0;
  _buffer.clear();
}

bool Image::resize(const size_type width, const size_type height,
                   const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
  if( width < 1  ||  height < 1 ) {
    return false;
  }

  _width  = width;
  _height = height;

  try {
    _buffer.resize(stride()*_height, 0xFF);
  } catch(...) {
    clear();
    return false;
  }

  fill(r, g, b, a);

  return true;
}

uint8_t *Image::row(const size_type y) const
{
  if( !isValidY(y) ) {
    return nullptr;
  }
  return const_cast<uint8_t*>(_buffer.data() + stride()*y);
}

void Image::fill(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
  for(size_type y = 0; y < height(); y++) {
    uint8_t *data = row(y);
    for(size_type x = 0; x < width(); x++) {
      *data++ = r;
      *data++ = g;
      *data++ = b;
      *data++ = a;
    }
  }
}

Image::size_type Image::stride() const
{
  return _width*4;
}

Image::size_type Image::width() const
{
  return _width;
}

Image::size_type Image::height() const
{
  return _height;
}

bool Image::copy(const size_type y, const Image& src)
{
  if( !isValidY(y)  ||  src.isEmpty()  ||
      stride() != src.stride()  ||  y + src.height() > height() ) {
    return false;
  }
  std::memcpy(row(y), src.row(0), src.height()*stride());
  return true;
}

bool Image::saveAsPNG(const char *filename) const
{
  if( isEmpty() ) {
    return false;
  }
  const int w = static_cast<int>(width());
  const int h = static_cast<int>(height());
  const int s = static_cast<int>(stride());
  return stbi_write_png(filename, w, h, STBI_COMP_RGBA8888, row(0), s) != 0;
}
