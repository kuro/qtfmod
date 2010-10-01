cmake_minimum_required(VERSION 2.8)

find_package(FMOD REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtTest REQUIRED)
include(${QT_USE_FILE})

add_definitions(-Wall -Wextra -Werror)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

subdirs(src test)
