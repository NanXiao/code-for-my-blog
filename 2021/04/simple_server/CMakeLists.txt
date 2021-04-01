cmake_minimum_required(VERSION 3.13)
project(simple_server C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/")
find_package(Libev REQUIRED)

add_executable(server main.c)
target_include_directories(server PUBLIC ${LIBEV_INCLUDE_DIRS})
target_link_libraries(server ${LIBEV_LIBRARY})
