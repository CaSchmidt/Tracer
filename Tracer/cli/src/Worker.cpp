/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <mutex>

#include "Worker.h"

template<typename CLOCK>
struct Elapsed {
  using      minutes = std::chrono::minutes;
  using      seconds = std::chrono::seconds;
  using milliseconds = std::chrono::milliseconds;

  using time_point = typename CLOCK::time_point;

  Elapsed(const time_point& begin, const time_point& end) noexcept
  {
    const milliseconds delta = std::chrono::duration_cast<milliseconds>(end - begin);
    min  = std::chrono::duration_cast<minutes>(delta);
    sec  = std::chrono::duration_cast<seconds>(delta % minutes(1));
    msec = std::chrono::duration_cast<milliseconds>(delta % seconds(1));
  }

  minutes       min{};
  seconds       sec{};
  milliseconds msec{};

private:
  Elapsed() noexcept = delete;
};

template<typename CLOCK, typename CharT, typename Traits>
std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>& os,
                                             const Elapsed<CLOCK>& elapsed)
{
  return os
      << elapsed.min.count()  << "m"
      << elapsed.sec.count()  << "s"
      << elapsed.msec.count() << "ms";
}

Image Worker::execute(const rt::IRenderer *renderer,
                      const rt::CameraPtr& camera, const rt::SamplerPtr& sampler,
                      const std::size_t blockSize) const
{
  Image image(renderer->options().width, renderer->options().height);
  if( image.isEmpty() ) {
    return Image();
  }

  const Blocks blocks = makeBlocks(image.height(), blockSize);
  if( blocks.empty() ) {
    return Image();
  }

  const auto tim_begin = std::chrono::high_resolution_clock::now();

  std::size_t done = 0;
  std::mutex mutex;
  std::for_each(std::execution::par_unseq,
                blocks.begin(), blocks.end(), [&](const Block& block) -> void {
    const rt::SamplerPtr mysampler = sampler->copy();
    const auto [y0, y1] = block;
    const Image slice = renderer->render(y0, y1, camera, mysampler);
    {
      std::lock_guard<std::mutex> lock(mutex);
      image.copy(y0, slice);
      done += y1 - y0;
      progress(done, image.height());
    }
  });

  const auto tim_end = std::chrono::high_resolution_clock::now();
  const Elapsed<std::chrono::high_resolution_clock> elapsed(tim_begin, tim_end);
  std::cout << "Duration: " << elapsed << std::endl;

  return image;
}

////// private ///////////////////////////////////////////////////////////////

Blocks Worker::makeBlocks(const std::size_t height, const std::size_t blockSize)
{
  Blocks blocks;

  const std::size_t numBlocks = height/blockSize;
  for(std::size_t i = 0; i < numBlocks; i++) {
    const std::size_t y0 = i*blockSize;
    blocks.emplace_back(y0, y0 + blockSize);
  }

  const std::size_t numRemain = height%blockSize;
  if( numRemain > 0 ) {
    const std::size_t y0 = numBlocks*blockSize;
    blocks.emplace_back(y0, y0 + numRemain);
  }

  return blocks;
}

void Worker::progress(const std::size_t y, const std::size_t height)
{
  const std::size_t p = (y*100)/height;
  printf("Progress: %3d%% (%6d/%6d)\n", int(p), int(y), int(height));
  fflush(stdout);
}
