cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE CLI_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

project(SibillaCLI)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(SblaCLI ${CLI_SRC})

target_include_directories(SblaCLI PRIVATE Header)

target_link_libraries(SblaCLI PRIVATE SblaCore)
target_link_libraries(SblaCLI PRIVATE Threads::Threads)
target_link_libraries(SblaCLI PRIVATE ${CMAKE_DL_LIBS})

find_package(tomlplusplus CONFIG REQUIRED)
    target_link_libraries(SblaCLI PRIVATE tomlplusplus::tomlplusplus)
    
find_path(DYLIB_INCLUDE_DIRS "dylib.hpp")
    target_include_directories(SblaCLI PRIVATE ${DYLIB_INCLUDE_DIRS})

install(TARGETS SblaCLI
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
