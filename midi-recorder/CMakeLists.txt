set(midi_recorder_SRCS
    midi-recorder.cpp
    mainwindow.cpp
    )

set(midi_recorder_HEADERS
    mainwindow.h
    )

set(midi_recorder_FORMS
    mainwindow.ui
    )

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
qt5_wrap_cpp(midi_recorder_HEADERS_MOC ${midi_recorder_HEADERS})
qt5_wrap_ui(midi_recorder_FORMS_HEADERS ${midi_recorder_FORMS})

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

add_executable(midi-recorder
    ${midi_recorder_SRCS} ${midi_recorder_HEADERS_MOC} ${midi_recorder_FORMS_HEADERS}
    )
    
target_link_libraries(midi-recorder
    libzynth
    )

qt5_use_modules(midi-recorder Widgets)