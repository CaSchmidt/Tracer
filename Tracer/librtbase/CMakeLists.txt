list(APPEND rtbase_HEADERS
  include/geom/Bounds.h
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
  include/rt/Loader/SceneLoaderBase.h
  include/rt/Loader/SceneLoaderStringUtil.h
  include/rt/Renderer/IRenderer.h
  include/rt/Renderer/RenderContext.h
  include/rt/Renderer/RenderLoop.h
  include/rt/Renderer/RenderOptions.h
  include/rt/Sampler/ISampler.h
  include/rt/Sampler/Sample.h
  include/rt/Sampler/Sampling.h
  include/rt/Sampler/SimpleSampler.h
  include/rt/Scene/IScene.h
  include/rt/Texture/CheckedTexture.h
  include/rt/Texture/FlatTexture.h
  include/rt/Texture/ITexture.h
  include/rt/Texture/TexCoord.h
  include/Util/Worker.h
  )

list(APPEND rtbase_SOURCES
  src/Camera/FrustumCamera.cpp
  src/Camera/ICamera.cpp
  src/Camera/SimpleCamera.cpp
  src/Loader/SceneLoaderBase.cpp
  src/Renderer/IRenderer.cpp
  src/Renderer/RenderContext.cpp
  src/Renderer/RenderOptionsLoader.cpp
  src/Sampler/ISampler.cpp
  src/Sampler/Sampling.cpp
  src/Sampler/SimpleSampler.cpp
  src/Scene/IScene.cpp
  src/Texture/CheckedTexture.cpp
  src/Texture/CheckedTextureLoader.cpp
  src/Texture/FlatTexture.cpp
  src/Texture/FlatTextureLoader.cpp
  src/Texture/ITexture.cpp
  src/Texture/ITextureLoader.cpp
  src-util/Worker.cpp
  )

add_library(rtbase STATIC
  ${rtbase_HEADERS}
  ${rtbase_SOURCES}
  )

format_output_name(rtbase "rtbase")

set_target_properties(rtbase PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_include_directories(rtbase
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(rtbase
  PUBLIC csUtil
  PUBLIC tinyxml2
  PUBLIC util
  )
