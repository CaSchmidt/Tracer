cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(Tracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Modules ##################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FormatOutputName)

### Dependencies #############################################################

add_subdirectory(../NumericArray/NumericArray
  ${CMAKE_CURRENT_BINARY_DIR}/NumericArray
  )

add_subdirectory(3rdparty/tinyxml2-800)

### Tracer ###################################################################

set(Tracer_HEADERS
  include/Image.h
  include/geom/Geom.h
  include/geom/Intersect.h
  include/geom/Optics.h
  include/geom/Ray.h
  include/geom/Transform.h
  include/math/Constants.h
  include/math/Logical.h
  include/math/Solver.h
  include/priv/rt/SceneLoaderBase.h
  include/priv/rt/SceneLoaderStringUtil.h
  include/rt/Camera.h
  include/rt/Color.h
  include/rt/Refraction.h
  include/rt/Renderer.h
  include/rt/SceneLoader.h
  include/rt/SurfaceInfo.h
  include/rt/Types.h
  include/rt/Light/DirectionalLight.h
  include/rt/Light/ILightSource.h
  include/rt/Light/PointLight.h
  include/rt/Material/IMaterial.h
  include/rt/Material/MirrorMaterial.h
  include/rt/Material/OpaqueMaterial.h
  include/rt/Material/TransparentMaterial.h
  include/rt/Object/Cylinder.h
  include/rt/Object/IObject.h
  include/rt/Object/Plane.h
  include/rt/Object/Sphere.h
  include/rt/Texture/CheckedTexture.h
  include/rt/Texture/FlatTexture.h
  include/rt/Texture/ITexture.h
  )

set(Tracer_SOURCES
  src/Image.cpp
  src/main.cpp
  src/rt/Camera.cpp
  src/rt/Refraction.cpp
  src/rt/Renderer.cpp
  src/rt/SceneLoader.cpp
  src/rt/SceneLoaderBase.cpp
  src/rt/SceneLoaderLight.cpp
  src/rt/SceneLoaderMaterial.cpp
  src/rt/SceneLoaderObject.cpp
  src/rt/SceneLoaderOptions.cpp
  src/rt/SceneLoaderText.cpp
  src/rt/SceneLoaderTexture.cpp
  src/rt/Light/DirectionalLight.cpp
  src/rt/Light/ILightSoruce.cpp
  src/rt/Light/PointLight.cpp
  src/rt/Material/IMaterial.cpp
  src/rt/Material/MirrorMaterial.cpp
  src/rt/Material/OpaqueMaterial.cpp
  src/rt/Material/TransparentMaterial.cpp
  src/rt/Object/Cylinder.cpp
  src/rt/Object/IObject.cpp
  src/rt/Object/Plane.cpp
  src/rt/Object/Sphere.cpp
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

format_output_name(Tracer "Tracer")

add_dependencies(Tracer NumericArray)

target_include_directories(Tracer PRIVATE
  ${Tracer_SOURCE_DIR}/3rdparty/font8x8/include
  ${Tracer_SOURCE_DIR}/3rdparty/stb/include
  ${NumericArray_SOURCE_DIR}/include
  ${Tracer_SOURCE_DIR}/include
  )

target_link_libraries(Tracer tinyxml2)
