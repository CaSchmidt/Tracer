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

### Project ##################################################################

add_subdirectory(libutil)
add_subdirectory(librt)

### Tracer ###################################################################

set(Tracer_HEADERS
  include/Worker.h
  )

set(Tracer_SOURCES
  src/main.cpp
  src/Worker.cpp
  )

### Target ###################################################################

add_executable(Tracer
  ${Tracer_HEADERS}
  ${Tracer_SOURCES}
  )

format_output_name(Tracer "Tracer")

add_dependencies(Tracer N4 tinyxml2 rt util)

target_include_directories(Tracer
  PRIVATE ${Tracer_SOURCE_DIR}/include
  )

target_link_libraries(Tracer
  PRIVATE rt
  )
