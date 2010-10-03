
project(QtFMOD)

set(sources
    QtFMOD/System.h
    QtFMOD/System.cpp
    QtFMOD/Sound.h
    QtFMOD/Sound.cpp
    QtFMOD/Channel.h
    QtFMOD/Channel.cpp
    QtFMOD/Tag.h
    QtFMOD/Tag.cpp
    QtFMOD/DSP.h
    QtFMOD/DSP.cpp
    QtFMOD/DSPConnection.h
    QtFMOD/DSPConnection.cpp
    fileio.h
    fileio.cpp
    )
qt4_automoc(${sources})
include_directories(
    ${FMOD_INCLUDE_DIRS}
    )
add_library(QtFMOD ${sources})
