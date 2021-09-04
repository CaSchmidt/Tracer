list(APPEND gui_FORMS
  forms/WMainWindow.ui
  )

list(APPEND gui_HEADERS
  include/Util.h
  include/WImage.h
  include/WMainWindow.h
  )

list(APPEND gui_SOURCES
  src/main.cpp
  src/Util.cpp
  src/WImage.cpp
  src/WMainWindow.cpp
  )

add_executable(gui
  ${gui_FORMS}
  ${gui_HEADERS}
  ${gui_SOURCES}
  )

format_output_name(gui "Tracer-GUI")

set_target_properties(gui PROPERTIES
  AUTOMOC ON
  AUTORCC ON
  AUTOUIC ON
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms
  )

if(WIN32)
  set_target_properties(gui PROPERTIES
    WIN32_EXECUTABLE ON
    )
endif()

target_compile_definitions(gui
  PRIVATE -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII
  )

target_include_directories(gui
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(gui Qt5::Widgets Qt5::Concurrent pt rt)
