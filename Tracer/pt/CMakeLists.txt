list(APPEND pt_HEADERS
  include/pt/Renderer/PathTracer.h
  include/pt/Scene/Scene.h
  )

list(APPEND pt_SOURCES
  src/Renderer/PathTracer.cpp
  src/Scene/Scene.cpp
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
