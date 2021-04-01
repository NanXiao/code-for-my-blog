cmake_minimum_required(VERSION 3.13)
project(https_client C)

find_package(OpenSSL REQUIRED)

add_executable(client main.c)
target_link_libraries(client ${OPENSSL_LIBRARIES})
