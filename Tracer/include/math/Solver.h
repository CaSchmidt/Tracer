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

#ifndef SOLVER_H
#define SOLVER_H

#include <cmath>

#include <algorithm>

#include <cs/Math.h>

#include "math/Constants.h"

namespace math {

  template<typename T>
  inline T abs(const T& x, const T& y)
  {
    return csSqrt(x*x + y*y);
  }

  template<typename T>
  constexpr T degree(const T& x)
  {
    return x*static_cast<T>(180)/PI<T>;
  }

  template<typename T>
  inline T phase(const T& x, const T& y)
  {
    // atan2(y,x) -> phi := [-pi,pi]
    T phi = csATan2(y, x);
    if( phi < ZERO<T> ) {
      phi += TWO_PI<T>;
    }
    return phi;
  }

  template<typename T>
  inline T pythagoras(const T& x)
  {
    // x^2 + y^2 = 1  =>  y = sqrt(1 - x^2)
    return csSqrt(csMax(ZERO<T>, ONE<T> - x*x));
  }

  template<typename T>
  inline bool quadratic(const T& _a, const T& _b, const T& _c,
                        T& x1, T& x2)
  {
    x1 = x2 = qNaN<T>;

    const double a = static_cast<double>(_a);
    const double b = static_cast<double>(_b);
    const double c = static_cast<double>(_c);

    const double discrim = b*b - FOUR<double>*a*c;
    if( discrim < ZERO<double> ) {
      return false;
    }

    const double rootDiscrim = csSqrt(discrim);
    const double q = b < ZERO<double>
        ? -ONE_HALF<double>*(b - rootDiscrim)
        : -ONE_HALF<double>*(b + rootDiscrim);

    x1 = static_cast<T>(q/a);
    x2 = static_cast<T>(c/q);

    if( x1 > x2 ) {
      std::swap(x1, x2);
    }

    return true;
  }

  template<typename T>
  constexpr T radian(const T& x)
  {
    return x*PI<T>/static_cast<T>(180);
  }

} // namespace math

#endif // SOLVER_H
