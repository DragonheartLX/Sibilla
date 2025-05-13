cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE BOT_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LizBot)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_definitions(SBL_BUILD_SHARED)
add_compile_definitions(SBL_BUILD_DLL)
add_compile_definitions(SBL_CHATBOT_BUILD)

add_library(LizBot SHARED ${BOT_SRC})

target_include_directories(LizBot PRIVATE Header)

target_link_libraries(LizBot PRIVATE SblaCore)

find_package(CURL REQUIRED)
    target_link_libraries(LizBot PRIVATE CURL::libcurl)

find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(LizBot PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS LizBot
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Bot
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Bot
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
