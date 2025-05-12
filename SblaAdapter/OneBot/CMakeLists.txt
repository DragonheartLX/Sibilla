cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE ADA_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OneBotAdapter)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_definitions(SBL_BUILD_SHARED)
add_compile_definitions(SBL_BUILD_DLL)
add_compile_definitions(SBL_ADAPTER_BUILD)

add_library(OneBotAdapter SHARED ${ADA_SRC})

target_include_directories(OneBotAdapter PRIVATE Header)

target_link_libraries(OneBotAdapter PRIVATE SblaCore)

install(TARGETS OneBotAdapter
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Adapter
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Adapter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
