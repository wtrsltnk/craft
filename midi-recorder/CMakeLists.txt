set(midi_recorder_SRCS
    program.cpp
    mainwindow.cpp
    instrumentselectiondialog.cpp
    mastermixerwidget.cpp
    midiclip.cpp
    midirecorder.cpp
    midiplayer.cpp
    noteview.cpp
    EffectsUI/ReverbForm.cpp
    EffectsUI/EchoForm.cpp
    EffectsUI/ChorusForm.cpp
    EffectsUI/PhaserForm.cpp
    EffectsUI/AlienWahForm.cpp
    EffectsUI/DistorsionForm.cpp
    EffectsUI/EQForm.cpp
    EffectsUI/DynFilterForm.cpp
    )

set(midi_recorder_HEADERS
    mainwindow.h
    instrumentselectiondialog.h
    mastermixerwidget.h
    midiclip.h
    midirecorder.h
    midiplayer.h
    noteview.h
    EffectsUI/ReverbForm.h
    EffectsUI/EchoForm.h
    EffectsUI/ChorusForm.h
    EffectsUI/PhaserForm.h
    EffectsUI/AlienWahForm.h
    EffectsUI/DistorsionForm.h
    EffectsUI/EQForm.h
    EffectsUI/DynFilterForm.h
    )

set(midi_recorder_FORMS
    mainwindow.ui
    mastermixerwidget.ui
    instrumentselectiondialog.ui
    EffectsUI/ReverbForm.ui
    EffectsUI/EchoForm.ui
    EffectsUI/ChorusForm.ui
    EffectsUI/PhaserForm.ui
    EffectsUI/AlienWahForm.ui
    EffectsUI/DistorsionForm.ui
    EffectsUI/EQForm.ui
    EffectsUI/DynFilterForm.ui
    )

SET(midi_recorder_RESOURCES
    resources.qrc
    )
    
IF(MINGW)
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/midiicon.o
                    COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/midiicon.rc 
                         -o ${CMAKE_CURRENT_BINARY_DIR}/midiicon.o )
    SET(midi_recorder_SRCS ${midi_recorder_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/midiicon.o)
ELSE(MINGW)
    SET(midi_recorder_SRCS ${midi_recorder_SRCS} midiicon.rc)
ENDIF(MINGW)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
qt5_wrap_cpp(midi_recorder_HEADERS_MOC ${midi_recorder_HEADERS})
qt5_wrap_ui(midi_recorder_FORMS_HEADERS ${midi_recorder_FORMS})
qt5_add_resources(midi_recorder_RESOURCES_HEADERS ${midi_recorder_RESOURCES})

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

add_executable(midi-recorder WIN32
    ${midi_recorder_SRCS} ${midi_recorder_HEADERS_MOC} ${midi_recorder_FORMS_HEADERS} ${midi_recorder_RESOURCES_HEADERS}
    )
    
target_link_libraries(midi-recorder
    libzynth
    )

qt5_use_modules(midi-recorder Widgets)
