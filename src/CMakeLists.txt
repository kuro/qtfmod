set(sources
    System.h
    System.cpp
    )
qt4_automoc(${sources})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )
add_library(QtFMOD ${sources})
