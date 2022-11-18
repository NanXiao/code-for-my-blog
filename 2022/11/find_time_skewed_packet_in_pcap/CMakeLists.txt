cmake_minimum_required(VERSION 3.13)
project(find_time_skewed_packet_in_pcap C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/")
find_package(PCAP REQUIRED)

add_executable(main main.c)
target_link_libraries(main ${PCAP_LIBRARY} )
