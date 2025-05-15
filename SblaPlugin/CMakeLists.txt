cmake_minimum_required(VERSION 3.19)
file(GLOB_RECURSE PLUGIN_SRC LIST_DIRECTORIES false
    Source/*.cpp
    Source/**/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SblaPlugin)

# ========================================
# Plugin Info
# ========================================
set(PLUGIN_NAME "SblaPlugin")
set(PLUGIN_VERSION "0.0.0")
set(PLUGIN_TYPE "ALL")

option(SBLA_GIT_COMMIT "Enable Git commit to Version" OFF)
option(SBLA_PLATFORM "Enable Sbla Platform" ON)
option(SBLA_CHATBOT "Enable Sbla Chatbot" ON)
# ========================================

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PLUGIN_VERSION_SUFFIX "Dev")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PLUGIN_VERSION_SUFFIX "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(PLUGIN_VERSION_SUFFIX "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(PLUGIN_VERSION_SUFFIX "Release")
else()
    set(PLUGIN_VERSION_SUFFIX "Dev")
endif()

set(PLUGIN_VERSION_FULL "${PLUGIN_VERSION}-${PLUGIN_VERSION_SUFFIX}")

configure_file(cmake/PluginConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Header/PluginConfig.h)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_definitions(SBL_BUILD_SHARED)
add_compile_definitions(SBL_BUILD_DLL)

add_library(SblaPlugin SHARED ${PLUGIN_SRC})
target_include_directories(SblaPlugin PRIVATE Header)
target_link_libraries(SblaPlugin PRIVATE SblaCore)

# ========================================
# link external library
# find_package(LIBA CONFIG REQUIRED)
#     target_link_libraries(PLUGIN PRIVATE LIBA)
# ========================================

# ========================================

install(TARGETS SblaPlugin
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Plugins
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Plugins
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
