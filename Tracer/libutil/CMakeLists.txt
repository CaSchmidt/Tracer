list(APPEND util_HEADERS
  include/Debug.h
  include/Image.h
  )

list(APPEND util_SOURCES
  src/Debug.cpp
  src/Image.cpp
  )

add_library(util STATIC
  ${util_HEADERS}
  ${util_SOURCES}
  )

format_output_name(util "util")

target_include_directories(util
  PRIVATE ${NumericArray_SOURCE_DIR}/include
  PRIVATE ${Tracer_SOURCE_DIR}/3rdparty/stb/include
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

add_dependencies(util N4)
