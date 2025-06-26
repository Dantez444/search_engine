cmake_minimum_required(VERSION 3.20)

project(search_server)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

add_subdirectory(src)
add_subdirectory(tests)
