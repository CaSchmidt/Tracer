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

#include <array>

#include "tests.h"

#include "rt/Camera.h"

constexpr rt::real_T scaledPos(const rt::real_T& in, const rt::real_T& s1, const rt::real_T& s2)
{
  return s2*(rt::TWO*(in + rt::ONE_HALF)/s1 - 1);
}

void pos(const rt::real_T x, const rt::real_T y,
         const rt::real_T w, const rt::real_T h)
{
  const rt::real_T a = w/h;

  const double d1 = static_cast<double>(scaledPos(x, w, a));
  const double d2 = static_cast<double>(scaledPos(y, h, -1));
  printf("(%6.3f,%6.3f)", d1, d2);
}

void print(const rt::Vec3f& v, const bool lf = false)
{
  printf("(%9.6f,%9.6f,%9.6f)",
         double(v.x), double(v.y), double(v.z));
  if( lf ) {
    printf("\n");
  }
}

void test_camera()
{
#if 1
  /*
   * NOTE:
   * Setting aspect=width/height makes aspect/width equal 1/height;
   * AKA the normalization yields square pixels!
   */

  const rt::dim_T  width = 4;
  const rt::dim_T height = 3;
  const double    aspect = double(width)/height;

  printf("aspect = %.6f\n", aspect);
  printf("   npw = %.6f\n", aspect*2.0/width);
  printf("   nph = %.6f\n",        2.0/height);

  rt::Camera cam({0, 0, 1}, {0, 0, 0}, {0, 1, 0}, width, height, rt::PI_HALF);
  for(rt::dim_T y = 0; y < height; y++) {
    for(rt::dim_T x = 0; x < width; x++) {
      const rt::Rayf  ray = cam.ray(x, y);
      const rt::Vec3f org = ray.origin();
      printf("  (%5.2f,%5.2f,%5.2f)",
             double(org.x), double(org.y), double(org.z));
    }
    printf("\n");
  }

  printf("\n");
  printf("\nCompute 2x2 RGSS...\n");
  printf("\n");

  const rt::real_T angle = std::atan(rt::ONE_HALF);
  printf("angle = %.3fdeg\n", double(angle)/M_PI*180.0);

  using Samples2x2 = std::array<rt::Vec3f,4>;
  using  size_type = Samples2x2::size_type;

  Samples2x2 samples;
  samples[0] = { -0.25,  0.25, 0 };
  samples[1] = {  0.25,  0.25, 0 };
  samples[2] = { -0.25, -0.25, 0 };
  samples[3] = {  0.25, -0.25, 0 };

  Samples2x2 rotSamples;
  for(size_type i = 0; i < samples.size(); i++) {
    rotSamples[i] = rt::Mat3f::rotateZ(angle)*samples[i];
  }

  for(size_type i = 0; i < samples.size(); i++) {
    print(samples[i]/rt::ONE_HALF);
    printf(" -> ");
    print(rotSamples[i]/rt::ONE_HALF);
    printf("\n");
  }

#else
  const rt::dim_T w = 7;
  const rt::dim_T h = 4;

  printf("ascpect = %.3f\n", double(w)/h);

  for(rt::dim_T y = 0; y < h; y++) {
    for(rt::dim_T x = 0; x < w; x++) {
      printf("  ");
      pos(x, y, w, h);
    }
    printf("\n");
  }
#endif
}
