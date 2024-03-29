list(APPEND pt_HEADERS
  include/pt/BSDF/Dielectric.h
  include/pt/BSDF/Diffuse.h
  include/pt/BSDF/IBSDF.h
  include/pt/BSDF/Mirror.h
  include/pt/Renderer/PathTracer.h
  include/pt/Scene/Object.h
  include/pt/Scene/Scene.h
  include/pt/Shape/Cylinder.h
  include/pt/Shape/Disk.h
  include/pt/Shape/IntersectionInfo.h
  include/pt/Shape/IShape.h
  include/pt/Shape/Plane.h
  include/pt/Shape/Sphere.h
  )

list(APPEND pt_SOURCES
  src/BSDF/Dielectric.cpp
  src/BSDF/DielectricLoader.cpp
  src/BSDF/Diffuse.cpp
  src/BSDF/DiffuseLoader.cpp
  src/BSDF/IBSDF.cpp
  src/BSDF/IBSDFLoader.cpp
  src/BSDF/Mirror.cpp
  src/BSDF/MirrorLoader.cpp
  src/Renderer/PathTracer.cpp
  src/Scene/Object.cpp
  src/Scene/ObjectLibrary.cpp
  src/Scene/ObjectLoader.cpp
  src/Scene/Scene.cpp
  src/Scene/SceneLoader.cpp
  src/Shape/Cylinder.cpp
  src/Shape/CylinderLoader.cpp
  src/Shape/Disk.cpp
  src/Shape/DiskLoader.cpp
  src/Shape/IntersectionInfo.cpp
  src/Shape/IShape.cpp
  src/Shape/IShapeLoader.cpp
  src/Shape/Plane.cpp
  src/Shape/PlaneLoader.cpp
  src/Shape/Sphere.cpp
  src/Shape/SphereLoader.cpp
  )

add_library(pt STATIC
  ${pt_HEADERS}
  ${pt_SOURCES}
  )

format_output_name(pt "pt")

set_target_properties(pt PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_include_directories(pt
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(pt
  PUBLIC rtbase
  )
