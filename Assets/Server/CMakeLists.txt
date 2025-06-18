cmake_minimum_required(VERSION 3.14)
project(Server)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/wrappers
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

file(GLOB_RECURSE UTIL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/**/*.cpp
)

file(GLOB_RECURSE WRAPPERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wrappers/**/*.cpp
)

set(ENTRY_POINT
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(SOURCES
    ${ENTRY_POINT}
    ${UTIL_SOURCES}
    ${WRAPPERS_SOURCES}
)

add_executable(Server ${SOURCES})