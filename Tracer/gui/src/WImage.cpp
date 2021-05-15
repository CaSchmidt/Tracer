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

#include <cstring>

#include <QtGui/QClipboard>
#include <QtGui/QGuiApplication>
#include <QtGui/QPainter>

#include "WImage.h"

#include "Image.h"

////// public ////////////////////////////////////////////////////////////////

WImage::WImage(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , _backgroundBrush(Qt::white, Qt::SolidPattern)
{
  setAutoFillBackground(false);
}

WImage::~WImage()
{
}

QBrush WImage::backgroundBrush() const
{
  return _backgroundBrush;
}

void WImage::setBackgroundBrush(const QBrush& brush)
{
  _backgroundBrush = brush;
  update();
}

void WImage::copyToClipboard() const
{
  if( _image.isNull() ) {
    return;
  }
  QGuiApplication::clipboard()->setImage(_image);
}

QImage WImage::image() const
{
  return _image;
}

void WImage::setImage(const QImage& image)
{
  _image = image;
  update();
}

bool WImage::insert(const Image::size_type y0, const Image& src)
{
  if( _image.isNull()  ||  src.isEmpty() ) {
    return false;
  }

  const int  srcBytes  = int(src.stride());
  const int  srcWidth  = int(src.width());
  const int  srcHeight = int(src.height());
  const int destY0     = int(y0);
  if( srcWidth != _image.width()  ||  destY0 + srcHeight > _image.height()  ||
      srcBytes != _image.bytesPerLine() ) {
    return false;
  }

  std::memcpy(_image.scanLine(destY0), src.row(0), src.height()*src.stride());

  update();

  return true;
}

bool WImage::saveAs(const QString& filename, const int quality) const
{
  if( _image.isNull() ) {
    return false;
  }
  return _image.save(filename, nullptr, quality);
}

////// protected /////////////////////////////////////////////////////////////

void WImage::paintEvent(QPaintEvent * /*event*/)
{
  QPainter painter(this);

  painter.fillRect(0, 0, width(), height(), _backgroundBrush);

  if( _image.isNull() ) {
    return;
  }

  const int ox = (width()  - _image.width() )/2;
  const int oy = (height() - _image.height())/2;

  painter.drawImage(ox, oy, _image);
}
