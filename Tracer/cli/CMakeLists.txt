set(cli_SOURCES
  src/main.cpp
  )

add_executable(cli
  ${cli_SOURCES}
  )

format_output_name(cli "Tracer-CLI")

set_target_properties(cli PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_link_libraries(cli
  PRIVATE rt
  )
