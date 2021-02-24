cmake_minimum_required(VERSION 3.13)
project(g_array_test C)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
add_executable(main main.c)
target_link_libraries(main ${GLIB_LDFLAGS})
