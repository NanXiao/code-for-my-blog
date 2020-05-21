cmake_minimum_required(VERSION 3.5)
project (asan_no_sanitize_address)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
add_executable(main main.c)
