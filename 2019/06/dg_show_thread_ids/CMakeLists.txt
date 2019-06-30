cmake_minimum_required(VERSION 3.13)
project(dg_show_thread_ids C)

set(CMAKE_C_STANDARD 11)

add_executable(dg_show_thread_ids main.c)

target_link_libraries(dg_show_thread_ids -lkvm)