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

#include "scenes.h"
#include "tests.h"

void print_progress(const int y, const int height, const bool force = false)
{
  if( y % 20 != 0  &&  !force ) {
    return;
  }
  const int p = (y*100)/height;
  printf("Progress: %3d%% (%4d/%4d)\n", p, y, height);
}

int main(int /*argc*/, char ** /*argv*/)
{
  rt::Renderer renderer;
#if 1
  initialize_scene_4(renderer, 1000, 1000);
#else
  initialize_scene_text(renderer, 2000, 2000,
                        "C++", 0.15, 0.3, 0.3, rt::Transformf::translate({0, 0, 1}));
#endif

  Image image(renderer.options().width, renderer.options().height);

  for(rt::dim_T y = 0; y < renderer.options().height; y++) {
    print_progress(y, renderer.options().height);
    renderer.render(y, image.row(y), 64);
  }
  print_progress(renderer.options().height, renderer.options().height, true);

  image.saveAsPNG("output.png");

  return EXIT_SUCCESS;
}
