cmake_minimum_required(VERSION 3.13)
project(spawn_threads)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(spawn_threads main.cpp)
target_link_libraries(spawn_threads Threads::Threads)