list(APPEND rt_HEADERS
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
  include/rt/Types.h
  include/rt/BxDF/IBxDF.h
  include/rt/BxDF/LambertianBRDF.h
  include/rt/BxDF/MirrorBRDF.h
  include/rt/BxDF/PhongBRDF.h
  include/rt/BxDF/SpecularReflectionBRDF.h
  include/rt/BxDF/SpecularTransmissionBTDF.h
  include/rt/Camera/FrustumCamera.h
  include/rt/Camera/ICamera.h
  include/rt/Camera/SimpleCamera.h
  include/rt/Light/DiffuseAreaLight.h
  include/rt/Light/DirectionalLight.h
  include/rt/Light/IAreaLight.h
  include/rt/Light/ILight.h
  include/rt/Light/PointLight.h
  include/rt/Loader/SceneLoader.h
  include/rt/Loader/SceneLoaderBase.h
  include/rt/Loader/SceneLoaderStringUtil.h
  include/rt/Material/BSDF.h
  include/rt/Material/IMaterial.h
  include/rt/Material/MatteMaterial.h
  include/rt/Material/MirrorMaterial.h
  include/rt/Material/OpaqueMaterial.h
  include/rt/Material/TransparentMaterial.h
  include/rt/Object/Cylinder.h
  include/rt/Object/Disk.h
  include/rt/Object/Group.h
  include/rt/Object/IObject.h
  include/rt/Object/Plane.h
  include/rt/Object/Sphere.h
  include/rt/Object/SurfaceInfo.h
  include/rt/Renderer/DirectLightingRenderer.h
  include/rt/Renderer/IRenderer.h
  include/rt/Renderer/PathTracingRenderer.h
  include/rt/Renderer/RenderContext.h
  include/rt/Renderer/RenderLoop.h
  include/rt/Renderer/RenderOptions.h
  include/rt/Renderer/RenderUtils.h
  include/rt/Renderer/WhittedRenderer.h
  include/rt/Sampler/ISampler.h
  include/rt/Sampler/Sample.h
  include/rt/Sampler/Sampling.h
  include/rt/Sampler/SimpleSampler.h
  include/rt/Scene/Scene.h
  include/rt/Texture/CheckedTexture.h
  include/rt/Texture/FlatTexture.h
  include/rt/Texture/ITexture.h
  include/rt/Texture/TexCoord.h
  )

list(APPEND rt_SOURCES
  src/BxDF/IBxDF.cpp
  src/BxDF/LambertianBRDF.cpp
  src/BxDF/MirrorBRDF.cpp
  src/BxDF/PhongBRDF.cpp
  src/BxDF/SpecularReflectionBRDF.cpp
  src/BxDF/SpecularTransmissionBTDF.cpp
  src/Camera/FrustumCamera.cpp
  src/Camera/ICamera.cpp
  src/Camera/SimpleCamera.cpp
  src/Light/DiffuseAreaLight.cpp
  src/Light/DirectionalLight.cpp
  src/Light/IAreaLight.cpp
  src/Light/ILight.cpp
  src/Light/PointLight.cpp
  src/Loader/SceneLoader.cpp
  src/Loader/SceneLoaderBase.cpp
  src/Loader/SceneLoaderLight.cpp
  src/Loader/SceneLoaderMaterial.cpp
  src/Loader/SceneLoaderObject.cpp
  src/Loader/SceneLoaderOptions.cpp
  src/Loader/SceneLoaderText.cpp
  src/Loader/SceneLoaderTexture.cpp
  src/Material/BSDF.cpp
  src/Material/IMaterial.cpp
  src/Material/MatteMaterial.cpp
  src/Material/MirrorMaterial.cpp
  src/Material/OpaqueMaterial.cpp
  src/Material/TransparentMaterial.cpp
  src/Object/Cylinder.cpp
  src/Object/Disk.cpp
  src/Object/Group.cpp
  src/Object/IObject.cpp
  src/Object/Plane.cpp
  src/Object/Sphere.cpp
  src/Object/SurfaceInfo.cpp
  src/Renderer/DirectLightingRenderer.cpp
  src/Renderer/IRenderer.cpp
  src/Renderer/PathTracingRenderer.cpp
  src/Renderer/RenderContext.cpp
  src/Renderer/RenderUtils.cpp
  src/Renderer/WhittedRenderer.cpp
  src/Sampler/ISampler.cpp
  src/Sampler/Sampling.cpp
  src/Sampler/SimpleSampler.cpp
  src/Scene/Scene.cpp
  src/Texture/CheckedTexture.cpp
  src/Texture/FlatTexture.cpp
  src/Texture/ITexture.cpp
  )

add_library(rt STATIC
  ${rt_HEADERS}
  ${rt_SOURCES}
  )

format_output_name(rt "rt")

target_include_directories(rt
  PRIVATE ${Tracer_SOURCE_DIR}/3rdparty/font8x8/include
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC  ${NumericArray_SOURCE_DIR}/include
  )

add_dependencies(rt N4 util tinyxml2)

target_link_libraries(rt
  PRIVATE tinyxml2
  PUBLIC  util
  )
