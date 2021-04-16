cmake_minimum_required(VERSION 3.13)
project(aes C)

find_package(OpenSSL REQUIRED)

add_executable(main main.c)
target_link_libraries(main ${OPENSSL_LIBRARIES})
