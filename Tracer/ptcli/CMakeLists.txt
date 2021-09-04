list(APPEND ptcli_SOURCES
  src/main.cpp
  )

add_executable(ptcli
  ${ptcli_SOURCES}
  )

format_output_name(ptcli "PathTracer-CLI")

target_link_libraries(ptcli
  PRIVATE pt
  )
