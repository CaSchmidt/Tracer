list(APPEND rtbase_HEADERS
  include/geom/Geom.h
  include/geom/Hit.h
  include/geom/Intersect.h
  include/geom/Optics.h
  include/geom/Ray.h
  include/geom/Shading.h
  include/geom/Transform.h
  include/geom/Util.h
  include/math/Constants.h
  include/math/Logical.h
  include/math/Solver.h
  include/rt/Base/Types.h
  include/rt/Camera/FrustumCamera.h
  include/rt/Camera/ICamera.h
  include/rt/Camera/SimpleCamera.h
  include/rt/Sampler/ISampler.h
  include/rt/Sampler/Sample.h
  include/rt/Sampler/Sampling.h
  include/rt/Sampler/SimpleSampler.h
  include/rt/Scene/IScene.h
  )

list(APPEND rtbase_SOURCES
  src/Camera/FrustumCamera.cpp
  src/Camera/ICamera.cpp
  src/Camera/SimpleCamera.cpp
  src/Sampler/ISampler.cpp
  src/Sampler/Sampling.cpp
  src/Sampler/SimpleSampler.cpp
  src/Scene/IScene.cpp
  )

add_library(rtbase STATIC
  ${rtbase_HEADERS}
  ${rtbase_SOURCES}
  )

format_output_name(rtbase "rtbase")

target_include_directories(rtbase
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${NumericArray_SOURCE_DIR}/include
  )

add_dependencies(rtbase N4)

target_link_libraries(rtbase)