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

#pragma once

#include <cstdint>

#include <vector>

class Image {
public:
  using    Buffer = std::vector<uint8_t>;
  using size_type = Buffer::size_type;

  Image() = default;
  ~Image() noexcept = default;

  Image(const size_type width, const size_type height,
        const uint8_t r = 0x00, const uint8_t g = 0x00, const uint8_t b = 0x00,
        const uint8_t a = 0xFF) noexcept;

  bool isEmpty() const;
  bool isValidX(const size_type x) const;
  bool isValidY(const size_type y) const;

  void clear();
  bool resize(const size_type width, const size_type height,
              const uint8_t r = 0x00, const uint8_t g = 0x00, const uint8_t b = 0x00,
              const uint8_t a = 0xFF);

  uint8_t *row(const size_type y) const;

  void fill(const uint8_t r, const uint8_t g, const uint8_t b,
            const uint8_t a = 0xFF);

  size_type stride() const;
  size_type width() const;
  size_type height() const;

  bool copy(const size_type y, const Image& src);

  bool saveAsPNG(const char *filename) const;

private:
  Buffer _buffer{};
  size_type _width{}, _height{};
};
