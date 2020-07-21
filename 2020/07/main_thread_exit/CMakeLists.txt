cmake_minimum_required(VERSION 3.13)
project(c_threads C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(main main.c)
target_link_libraries(main Threads::Threads)
