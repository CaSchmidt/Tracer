list(APPEND ptcli_SOURCES
  src/main.cpp
  )

add_executable(ptcli
  ${ptcli_SOURCES}
  )

format_output_name(ptcli "PathTracer-CLI")

set_target_properties(ptcli PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )

target_link_libraries(ptcli
  PRIVATE pt
  )
