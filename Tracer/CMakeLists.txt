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

add_subdirectory(../csUtil/csUtil
  ${CMAKE_CURRENT_BINARY_DIR}/csUtil
  )

add_subdirectory(3rdparty/tinyxml2-800)

### Project ##################################################################

add_subdirectory(libutil)
add_subdirectory(librtbase)
add_subdirectory(libpt)
add_subdirectory(librt)

add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(ptcli)

### Tests ####################################################################

add_subdirectory(tests)
