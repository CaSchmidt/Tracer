set(cli_HEADERS
  include/Worker.h
  )

set(cli_SOURCES
  src/main.cpp
  src/Worker.cpp
  )

add_executable(cli
  ${cli_HEADERS}
  ${cli_SOURCES}
  )

format_output_name(cli "Tracer-CLI")

target_include_directories(cli
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(cli
  PRIVATE rt
  )
