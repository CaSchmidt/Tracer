list(APPEND util_HEADERS
  include/Debug.h
  include/Image.h
  include/ValueObserver.h
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

set_target_properties(util PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_include_directories(util
  PRIVATE ${Tracer_SOURCE_DIR}/3rdparty/stb/include
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(util
  PRIVATE csUtil
  )
