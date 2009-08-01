set(sources
    System.h
    System.cpp
    Sound.h
    Sound.cpp
    Channel.h
    Channel.cpp
    fileio.h
    fileio.cpp
    )
qt4_automoc(${sources})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )
add_library(QtFMOD ${sources})
