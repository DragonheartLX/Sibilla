cmake_minimum_required(VERSION 3.25)
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

target_include_directories(SibillaCLI PRIVATE SibillaCLI)

find_package(CURL REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE CURL::libcurl)

find_package(fmt CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE fmt::fmt)

find_package(tomlplusplus CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE tomlplusplus::tomlplusplus)

find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE nlohmann_json::nlohmann_json)

find_package(absl CONFIG REQUIRED)
    target_link_libraries(SibillaCLI PRIVATE absl::base absl::random_random absl::strings)

install(TARGETS SibillaCLI
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
