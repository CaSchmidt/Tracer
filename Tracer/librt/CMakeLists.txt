list(APPEND rt_HEADERS
  include/rt/BxDF/IBxDF.h
  include/rt/BxDF/LambertianBRDF.h
  include/rt/BxDF/MirrorBRDF.h
  include/rt/BxDF/PhongBRDF.h
  include/rt/BxDF/SpecularReflectionBRDF.h
  include/rt/BxDF/SpecularTransmissionBTDF.h
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
  include/rt/Renderer/BaseRenderer.h
  include/rt/Renderer/DirectLightingRenderer.h
  include/rt/Renderer/PathTracingRenderer.h
  include/rt/Renderer/RenderUtils.h
  include/rt/Renderer/WhittedRenderer.h
  include/rt/Scene/Scene.h
  )

list(APPEND rt_SOURCES
  src/BxDF/IBxDF.cpp
  src/BxDF/LambertianBRDF.cpp
  src/BxDF/MirrorBRDF.cpp
  src/BxDF/PhongBRDF.cpp
  src/BxDF/SpecularReflectionBRDF.cpp
  src/BxDF/SpecularTransmissionBTDF.cpp
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
  src/Renderer/BaseRenderer.cpp
  src/Renderer/DirectLightingRenderer.cpp
  src/Renderer/PathTracingRenderer.cpp
  src/Renderer/RenderUtils.cpp
  src/Renderer/WhittedRenderer.cpp
  src/Scene/Scene.cpp
  )

add_library(rt STATIC
  ${rt_HEADERS}
  ${rt_SOURCES}
  )

format_output_name(rt "rt")

target_include_directories(rt
  PRIVATE ${Tracer_SOURCE_DIR}/3rdparty/font8x8/include
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(rt
  PUBLIC  rtbase
  PRIVATE tinyxml2
  )
