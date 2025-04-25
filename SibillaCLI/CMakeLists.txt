cmake_minimum_required(VERSION 3.28)
file(GLOB_RECURSE CLI_SRC LIST_DIRECTORIES false 
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 23)

project(SibillaCLI)

add_executable(SibillaCLI ${CLI_SRC})

target_include_directories(SibillaCLI PRIVATE SibillaCLI)

find_package(CURL REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE CURL::libcurl)

install(TARGETS SibillaCLI
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)