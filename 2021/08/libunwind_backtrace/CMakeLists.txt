cmake_minimum_required(VERSION 3.13)
project(libunwind_backtrace C)

find_package(PkgConfig REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/")
# https://raw.githubusercontent.com/m-a-d-n-e-s-s/madness/master/cmake/modules/FindLibunwind.cmake
find_package(Libunwind REQUIRED)

include_directories(${LIBUNWIND_INCLUDE_DIR})
add_executable(libunwind_backtrace libunwind_backtrace.c)
target_link_libraries(libunwind_backtrace ${LIBUNWIND_LIBRARIES})
