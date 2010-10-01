create_test_sourcelist(
    file_list driver.cpp fmod.cpp
    )
qt4_automoc(${file_list})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${FMOD_INCLUDE_DIRS}
    )
add_executable(driver ${file_list})
target_link_libraries(driver QtFMOD ${QT_LIBRARIES} ${FMOD_LIBRARIES})

set(tests ${file_list})
remove(tests driver.cpp)
foreach (test ${tests})
    get_filename_component(name ${test} NAME_WE)
    add_test(${name} ${EXECUTABLE_OUTPUT_PATH}/driver ${name})
endforeach()
