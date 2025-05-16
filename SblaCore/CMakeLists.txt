cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE CORE_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SibillaCore)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(SblaCore STATIC ${CORE_SRC})
set_target_properties(SblaCore PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(SblaCore PUBLIC Header)

find_package(OpenSSL REQUIRED)
    target_link_libraries(SblaCore PRIVATE OpenSSL::SSL)
    target_link_libraries(SblaCore PRIVATE OpenSSL::Crypto)

find_package(libhv CONFIG REQUIRED)
    target_link_libraries(SblaCore PUBLIC hv_static)

install(TARGETS SblaCore
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
