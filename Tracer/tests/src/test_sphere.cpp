#include <cstdio>
#include <cstdlib>

#include "rt/Object/Sphere.h"
#include "rt/Object/SurfaceInfo.h"

template<typename VecT>
void print(const VecT& v, FILE *file = stdout)
{
  fprintf(file, "%.6f %.6f %.6f\n", v(0), v(1), v(2));
  fflush(file);
}

int main(int /*argc*/, char ** /*argv*/)
{
  using rt::real_t;
  using rt::size_t;

  constexpr size_t   n = 8;
  constexpr real_t dXi = rt::ONE/real_t{n};

  constexpr size_t ni = n;
  constexpr size_t nj = n;

  const rt::ObjectPtr sphere = rt::Sphere::create(n4::translate(0, 0, 5), 1);

  rt::SurfaceInfo ref;
  ref.P = rt::Vertex();

  print(ref.P);

  for(size_t i = 0; i <= ni; i++) {
    const real_t si = i < ni
        ? dXi
        : dXi*(rt::ONE - 0x1p-10f);
    for(size_t j = 0; j <= nj; j++) {
      const real_t sj = j < nj
          ? dXi
          : dXi*(rt::ONE - 0x1p-10f);

      const rt::Sample2D xi{real_t(i)*si, real_t(j)*sj};

      const rt::SurfaceInfo surface = sphere->sample(ref, xi, nullptr);

      print(surface.P);
    }
  }

  return EXIT_SUCCESS;
}
