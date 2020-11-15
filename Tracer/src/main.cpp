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

#include <cstdio>
#include <cstdlib>

#include "Image.h"

#include "rt/Camera/SimpleCamera.h"
#include "rt/Renderer.h"
#include "rt/SceneLoader.h"

#define BASE_PATH  "../../Tracer/Tracer/scenes/"
#define FILE_1     BASE_PATH "scene_1.xml"
#define FILE_2     BASE_PATH "scene_2.xml"
#define FILE_3     BASE_PATH "scene_3.xml"
#define FILE_4     BASE_PATH "scene_4.xml"
#define FILE_TEXT  BASE_PATH "scene_text.xml"

void print_progress(const std::size_t y, const std::size_t height,
                    const bool force = false, const std::size_t blockSize = 20)
{
  if( y % std::max<std::size_t>(blockSize, 1) != 0  &&  !force ) {
    return;
  }
  const std::size_t p = (y*100)/height;
  printf("Progress: %3d%% (%6d/%6d)\n", int(p), int(y), int(height)); fflush(stdout);
}

void render_block(Image& image, const rt::ICamera *cam,
                  const std::size_t y0, const std::size_t y1,
                  const rt::Renderer& renderer, const std::size_t numSamples)
{
  const Image block = cam->render(image.width(), image.height(), y0, y1, renderer, numSamples);
  image.copy(y0, block);
  print_progress(y1, image.height(), true);
}

int main(int /*argc*/, char ** /*argv*/)
{
  const char *filename = FILE_1;

  rt::Renderer renderer;
  if( !rt::loadScene(renderer, filename) ) {
    return EXIT_FAILURE;
  }

  rt::SimpleCamera cam;
  cam.setup(renderer.options().fov_rad);

  constexpr std::size_t  blockSize = 32;
  constexpr std::size_t numSamples = 64;

  const std::size_t  width = renderer.options().width;
  const std::size_t height = renderer.options().height;
  Image image(width, height);

  const std::size_t numBlocks = height/blockSize;
  for(std::size_t i = 0; i < numBlocks; i++) {
    const std::size_t y = i*blockSize;
    render_block(image, &cam, y, y + blockSize, renderer, numSamples);
  }

  const std::size_t numRemain = height%blockSize;
  if( numRemain > 0 ) {
    const std::size_t y = numBlocks*blockSize;
    render_block(image, &cam, y, y + numRemain, renderer, numSamples);
  }

  image.saveAsPNG("output.png");

  return EXIT_SUCCESS;
}
