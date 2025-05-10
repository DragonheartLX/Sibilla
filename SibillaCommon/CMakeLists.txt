cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE COM_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SibillaCommon)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(SibillaCommon STATIC ${COM_SRC})

target_include_directories(SibillaCommon PRIVATE Header)

install(TARGETS SibillaCommon
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
