cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE CLI_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SibillaCLI)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(SibillaCLI ${CLI_SRC})

target_include_directories(SibillaCLI PRIVATE Header)

find_path(DYLIB_INCLUDE_DIRS "dylib.hpp")
    target_include_directories(SibillaCLI PRIVATE ${DYLIB_INCLUDE_DIRS})

target_link_libraries(SibillaCLI PRIVATE SibillaCommon)

install(TARGETS SibillaCLI
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
