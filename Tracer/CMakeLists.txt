cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(Tracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Modules ##################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FormatOutputName)

### Dependencies #############################################################

find_package(Qt5Widgets 5.6 REQUIRED)
find_package(Qt5Concurrent 5.6 REQUIRED)

add_subdirectory(../NumericArray/NumericArray
  ${CMAKE_CURRENT_BINARY_DIR}/NumericArray
  )

add_subdirectory(3rdparty/tinyxml2-800)

### Project ##################################################################

add_subdirectory(libutil)
add_subdirectory(librt)

add_subdirectory(cli)
add_subdirectory(gui)

### Tests ####################################################################

add_subdirectory(tests)
