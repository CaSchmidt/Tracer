set(cli_SOURCES
  src/main.cpp
  )

add_executable(cli
  ${cli_SOURCES}
  )

format_output_name(cli "Tracer-CLI")

target_link_libraries(cli
  PRIVATE rt
  )
