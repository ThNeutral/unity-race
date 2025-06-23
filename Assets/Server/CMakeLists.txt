cmake_minimum_required(VERSION 3.14)
project(Server)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/wrappers
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/deps
)

file(GLOB_RECURSE UTIL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities/**/*.cpp
)

file(GLOB_RECURSE WRAPPERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wrappers/**/*.cpp
)

file(GLOB_RECURSE GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/game/**/*.cpp
)

file(GLOB_RECURSE DEPS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/**/*.cpp
)

set(ENTRY_POINT
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(TEST_ENTRY_POINT
    ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp
)

set(SOURCES
    ${ENTRY_POINT}
    ${UTIL_SOURCES}
    ${WRAPPERS_SOURCES}
    ${DEPS_SOURCES}
    ${GAME_SOURCES}
)

set(SOURCES_TEST
    ${TEST_ENTRY_POINT}
    ${UTIL_SOURCES}
    ${WRAPPERS_SOURCES}
    ${DEPS_SOURCES}
    ${GAME_SOURCES}
)

add_executable(server ${SOURCES})
add_executable(server_test ${SOURCES_TEST})