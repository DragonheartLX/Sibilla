cmake_minimum_required(VERSION 3.25)
file(GLOB_RECURSE CLI_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SibillaCLI)

add_executable(SibillaCLI ${CLI_SRC})

target_include_directories(SibillaCLI PRIVATE SibillaCLI)

find_package(CURL REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE CURL::libcurl)

find_package(fmt CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE fmt::fmt)

find_package(tomlplusplus CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE tomlplusplus::tomlplusplus)

install(TARGETS SibillaCLI
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
