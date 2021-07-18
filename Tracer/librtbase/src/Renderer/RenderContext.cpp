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

#include "rt/Renderer/RenderContext.h"

namespace rt {

  ////// Public //////////////////////////////////////////////////////////////

  RenderBlocks makeRenderBlocks(const size_t height, const size_t blockSize)
  {
    RenderBlocks blocks;

    const size_t numBlocks = height/blockSize;
    for(size_t i = 0; i < numBlocks; i++) {
      const size_t y0 = i*blockSize;
      blocks.emplace_back(y0, y0 + blockSize);
    }

    if( const size_t numRemain = height%blockSize; numRemain > 0 ) {
      const size_t y0 = numBlocks*blockSize;
      blocks.emplace_back(y0, y0 + numRemain);
    }

    return blocks;
  }

  ////// RenderContext - public //////////////////////////////////////////////

  void RenderContext::clear()
  {
    camera.reset();
    renderer.reset();
    sampler.reset();
    scene.reset();
  }

  bool RenderContext::isValid() const
  {
    return camera  &&  renderer  &&  sampler  &&  scene;
  }

  Image RenderContext::render(const RenderBlock& block) const
  {
    const SamplerPtr mysampler = sampler->copy();
    const auto [y0, y1] = block;
    return renderer->render(y0, y1, scene, camera, mysampler);
  }

} // namespace rt
