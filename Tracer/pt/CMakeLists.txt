list(APPEND pt_HEADERS
  include/pt/Renderer/PathTracer.h
  include/pt/Scene/Scene.h
  include/pt/Shape/IntersectionInfo.h
  include/pt/Shape/IShape.h
  include/pt/Shape/Plane.h
  )

list(APPEND pt_SOURCES
  src/Renderer/PathTracer.cpp
  src/Scene/Scene.cpp
  src/Shape/IntersectionInfo.cpp
  src/Shape/IShape.cpp
  src/Shape/Plane.cpp
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
