list(APPEND tinyxml2_FILES
  src/include/tinyxml2.h
  src/src/tinyxml2.cpp
  )

add_library(tinyxml2 STATIC ${tinyxml2_FILES})

format_output_name(tinyxml2 "tinyxml2")

target_include_directories(tinyxml2
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  )
