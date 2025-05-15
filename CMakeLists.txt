cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Sibilla)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
set(SBL_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(SblaCore)
add_subdirectory(SblaPlugin)
add_subdirectory(SblaCLI)

add_custom_target(SblaSandBox
    DEPENDS install
    COMMENT "install for sandbox"
    COMMENT "Copy to sandbox"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_INSTALL_PREFIX}/bin ${SBL_ROOT_PATH}/SblaSandBox
)