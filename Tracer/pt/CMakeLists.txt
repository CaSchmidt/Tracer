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
  src/BSDF/Diffuse.cpp
  src/BSDF/IBSDF.cpp
  src/BSDF/Mirror.cpp
  src/Renderer/PathTracer.cpp
  src/Scene/Object.cpp
  src/Scene/ObjectLibrary.cpp
  src/Scene/Scene.cpp
  src/Shape/Cylinder.cpp
  src/Shape/Disk.cpp
  src/Shape/IntersectionInfo.cpp
  src/Shape/IShape.cpp
  src/Shape/Plane.cpp
  src/Shape/Sphere.cpp
  src/main.cpp
  )

add_executable(pt
  ${pt_HEADERS}
  ${pt_SOURCES}
  )

format_output_name(pt "PathTrace")

target_include_directories(pt
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(pt
  PRIVATE rtbase
  )
