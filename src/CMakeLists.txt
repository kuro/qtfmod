set(sources
    System.h
    System.cpp
    )
qt4_automoc(${sources})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${FMOD_INCLUDE_DIRS}
    )
add_library(QtFMOD ${sources})
