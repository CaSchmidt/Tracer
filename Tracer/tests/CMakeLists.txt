macro(cs_test target)
  set(sources ${ARGN})
  add_executable(${target} ${sources})
  format_output_name(${target} ${target})
  target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
  target_link_libraries(${target}
    PRIVATE rt
    )
endmacro()

### Tests ####################################################################

cs_test(test_sphere src/test_sphere.cpp)
