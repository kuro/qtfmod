cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(FMOD REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtTest REQUIRED)
include(${QT_USE_FILE})
include_directories(${FMOD_INCLUDE_DIRS})

subdirs(src test)
