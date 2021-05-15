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

#include "Util.h"

QBrush createCheckedBrush(const int size,
                          const QColor& color0, const QColor& color1)
{
  if( size < 1 ) {
    return QBrush();
  }

  QImage image(size*2, size*2, QImage::Format_RGBA8888);
  for(int y = 0; y < image.height(); y++) {
    const int pat1 = (y/size) != 0;

    uchar *row = image.scanLine(y);
    for(int x = 0; x < image.width(); x++) {
      const int pat2 = (x/size) != 0;

      uchar *r = row + x*4 + 0;
      uchar *g = row + x*4 + 1;
      uchar *b = row + x*4 + 2;
      uchar *a = row + x*4 + 3;

      if( (pat1 ^ pat2) != 0 ) {
        *r = color1.red();
        *g = color1.green();
        *b = color1.blue();
      } else {
        *r = color0.red();
        *g = color0.green();
        *b = color0.blue();
      }

      *a = 0xFF;
    }
  }

  QBrush brush;
  brush.setTextureImage(image);

  return brush;
}

QBrush createCheckedBrush(const int size, const int dark, const int light)
{
  const int _dark = qBound(0, dark, 255);
  const int _light = qBound(0, light, 255);
  return createCheckedBrush(size, QColor(_light, _light, _light), QColor(_dark, _dark, _dark));
}
