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

#include "tests.h"

#include "geom/Optics.h"
#include "rt/Types.h"

template<typename T>
inline double print_prep(const T& value)
{
  return std::abs(value) == T{0}
  ? static_cast<double>(0)
  : static_cast<double>(value);
}

template<typename T, typename DerivedT>
inline void print(const geom::VectorBase<T,DerivedT>& v, const char *text = nullptr)
{
  if( text != nullptr ) {
    printf("%s = \n", text);
  }

  const double x = print_prep(v.x);
  const double y = print_prep(v.y);
  const double z = print_prep(v.z);

  printf("    %8.3f\n", x);
  printf("    %8.3f\n", y);
  printf("    %8.3f\n", z);
}

void print(const rt::Matrix3f& M, const char *text = nullptr)
{
  if( text != nullptr ) {
    printf("%s = \n", text);
  }

  const double m00 = print_prep(M.m00);
  const double m01 = print_prep(M.m01);
  const double m02 = print_prep(M.m02);

  const double m10 = print_prep(M.m10);
  const double m11 = print_prep(M.m11);
  const double m12 = print_prep(M.m12);

  const double m20 = print_prep(M.m20);
  const double m21 = print_prep(M.m21);
  const double m22 = print_prep(M.m22);

  printf("    %8.3f  %8.3f  %8.3f\n", m00, m01, m02);
  printf("    %8.3f  %8.3f  %8.3f\n", m10, m11, m12);
  printf("    %8.3f  %8.3f  %8.3f\n", m20, m21, m22);
}

void print(const rt::Transformf& M, const char *text = nullptr)
{
  if( text != nullptr ) {
    printf("%s = \n", text);
  }

  const double m00 = print_prep(M.scaledRotation().m00);
  const double m01 = print_prep(M.scaledRotation().m01);
  const double m02 = print_prep(M.scaledRotation().m02);

  const double m10 = print_prep(M.scaledRotation().m10);
  const double m11 = print_prep(M.scaledRotation().m11);
  const double m12 = print_prep(M.scaledRotation().m12);

  const double m20 = print_prep(M.scaledRotation().m20);
  const double m21 = print_prep(M.scaledRotation().m21);
  const double m22 = print_prep(M.scaledRotation().m22);

  const double x = print_prep(M.translation().x);
  const double y = print_prep(M.translation().y);
  const double z = print_prep(M.translation().z);

  printf("    %8.3f  %8.3f  %8.3f  %8.3f\n", m00, m01, m02, x);
  printf("    %8.3f  %8.3f  %8.3f  %8.3f\n", m10, m11, m12, y);
  printf("    %8.3f  %8.3f  %8.3f  %8.3f\n", m20, m21, m22, z);
}

void inverse(const rt::Matrix3f& M, const char *text, const char *invText)
{
  print(M, text);
  print(M.inverse(), invText);
}

void test_matrix()
{
  const rt::Matrix3f A{3, 1, 1, 5, 2, 1, 3, 1, 2};
  inverse(A, "A", "A^-1");

  inverse(rt::Matrix3f::identity(), "I", "I^-1");

  inverse(rt::Matrix3f::scale(5, 5, 5), "S", "S^-1");

  print(rt::Matrix3f::rotateX(rt::PI_HALF), "rotateX(90)");
  print(rt::Matrix3f::rotateY(rt::PI_HALF), "rotateY(90)");
  print(rt::Matrix3f::rotateZ(rt::PI_HALF), "rotateZ(90)");

  const rt::Matrix3f M{1, 2, 3, 4, 5, 6, 7, 8, 9};
  print(M, "M");
  print(M + M, "M + M");
  print(M - M, "M - M");
  print(M - rt::TWO*M, "M - 2*M");
  print(M - M*rt::TWO, "M - M*2");
  print(M - M/rt::TWO, "M - M/2");
  print(M*rt::Matrix3f::identity(), "M*I");
  print(rt::Matrix3f::identity()*M, "I*M");
  bool ok = false;
  print(A*A.inverse(&ok), "A*A^-1");
  printf("ok = %s\n", ok ? "true" : "false");
  print(rt::Matrix3f::identity()*rt::Vertex3f{1,2,3}, "I*v");
  print(M.transposed(), "M^T");
  rt::Matrix3f M2{M};
  M2.transpose();
  print(M2, "M2^T");

  printf("****************************************************************\n");

  const rt::Matrix3f world2Cam = rt::Matrix3f::rotateX(-rt::PI_HALF);

  const rt::Vertex3f p1{1,1,1};
  print(world2Cam*p1, "p1");

  const rt::Vertex3f p2{1,-1,1};
  print(world2Cam*p2, "p2");
}

void test_optics()
{
  const rt::Normal3f I = rt::Normal3f{1, 0, -1}.normalized();
  print(I, "I");

  const rt::Normal3f N{0, 0, 1};
  print(N, "N");

  printf("dot(I,N) = %.3f\n", double(geom::dot(I, N)));

  const rt::Normal3f T = geom::refract(I, N, rt::real_T(1.333));
  print(T, "T");
  printf("length(T) = %.3f\n", double(T.length()));
}

void test_transform()
{
  const rt::Matrix3f X{1, 2, 3, 5, 6, 7, 9, 10, 11};
  print(X, "X");
  const rt::Vertex3f t{4, 8, 12};
  print(t, "t");

  print(rt::Transformf{X, t}, "M");

  const rt::Transformf M1{X};
  print(M1, "M1");
  const rt::Transformf M2{rt::Matrix3f::identity(), t};
  print(M2, "M2");

  print(M2*M1, "M2*M1");
  print(M1*M2, "M1*M2");
}

void test_vector()
{
  rt::Vertex3f v;
  float f = v.length();
  float a = 1 + f;
  rt::Vertex3f u = v.normalized();
  u.normalize();
  a = geom::dot01(u, v);
  rt::Vertex3f w = geom::cross(u, v);

  a = geom::distance(u, v);
  w = geom::direction(u, v);
}
