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

Image Worker::execute(const rt::RenderContext& rc, const rt::size_t blockSize) const
{
  Image image(rc.camera->width(), rc.camera->height());
  if( image.isEmpty() ) {
    return Image();
  }

  const rt::RenderBlocks blocks = rt::makeRenderBlocks(image.height(), blockSize);
  if( blocks.empty() ) {
    return Image();
  }

  const auto tim_begin = std::chrono::high_resolution_clock::now();

  rt::size_t done = 0;
  std::mutex mutex;
  std::for_each(std::execution::par_unseq,
                blocks.begin(), blocks.end(), [&](const rt::RenderBlock& block) -> void {
    const Image slice = rc.render(block);
    {
      std::lock_guard<std::mutex> lock(mutex);
      const auto [y0, y1] = block;
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

void Worker::progress(const rt::size_t y, const rt::size_t height)
{
  const rt::size_t p = (y*100)/height;
  printf("Progress: %3d%% (%6d/%6d)\n", int(p), int(y), int(height));
  fflush(stdout);
}
