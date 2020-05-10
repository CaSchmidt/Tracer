cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(Tracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Dependencies #############################################################

add_subdirectory(../NumericArray/NumericArray
  ${CMAKE_CURRENT_BINARY_DIR}/NumericArray
  )

### Tracer ###################################################################

set(Tracer_HEADERS
  include/Image.h
  include/scenes.h
  include/tests.h
  include/geom/Geom.h
  include/geom/GeomBase.h
  include/geom/Intersect.h
  include/geom/Optics.h
  include/geom/Ray.h
  include/geom/Transform.h
  include/math/Logical.h
  include/math/Pythagoras.h
  include/math/Quadratic.h
  include/rt/Camera.h
  include/rt/Color.h
  include/rt/Refraction.h
  include/rt/Renderer.h
  include/rt/SurfaceInfo.h
  include/rt/Types.h
  include/rt/Light/DirectionalLight.h
  include/rt/Light/ILightSource.h
  include/rt/Light/PointLight.h
  include/rt/Material/IMaterial.h
  include/rt/Material/MirrorMaterial.h
  include/rt/Material/OpaqueMaterial.h
  include/rt/Material/TransparentMaterial.h
  include/rt/Scene/Cylinder.h
  include/rt/Scene/IObject.h
  include/rt/Scene/Plane.h
  include/rt/Scene/Sphere.h
  include/rt/Texture/CheckedTexture.h
  include/rt/Texture/FlatTexture.h
  include/rt/Texture/ITexture.h
  )

set(Tracer_SOURCES
  src/Image.cpp
  src/main.cpp
  src/scene_1.cpp
  src/scene_2.cpp
  src/scene_3.cpp
  src/scene_4.cpp
  src/scene_text.cpp
  src/rt/Camera.cpp
  src/rt/Refraction.cpp
  src/rt/Renderer.cpp
  src/rt/Light/DirectionalLight.cpp
  src/rt/Light/ILightSoruce.cpp
  src/rt/Light/PointLight.cpp
  src/rt/Material/IMaterial.cpp
  src/rt/Material/MirrorMaterial.cpp
  src/rt/Material/OpaqueMaterial.cpp
  src/rt/Material/TransparentMaterial.cpp
  src/rt/Scene/Cylinder.cpp
  src/rt/Scene/IObject.cpp
  src/rt/Scene/Plane.cpp
  src/rt/Scene/Sphere.cpp
  src/rt/Texture/CheckedTexture.cpp
  src/rt/Texture/FlatTexture.cpp
  src/rt/Texture/ITexture.cpp
  )

set(Tracer_3rdparty
  3rdparty/font8x8/include/font8x8_basic.h
  3rdparty/stb/include/stb_image_write.h
  )

### Target ###################################################################

add_executable(Tracer
  ${Tracer_HEADERS}
  ${Tracer_SOURCES}
  ${Tracer_3rdparty}
  )

add_dependencies(Tracer NumericArray)

target_include_directories(Tracer PRIVATE
  ${Tracer_SOURCE_DIR}/3rdparty/font8x8/include
  ${Tracer_SOURCE_DIR}/3rdparty/stb/include
  ${NumericArray_SOURCE_DIR}/include
  ${Tracer_SOURCE_DIR}/include
  )

target_link_libraries(Tracer)
