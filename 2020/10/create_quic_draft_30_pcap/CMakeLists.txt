cmake_minimum_required(VERSION 3.13)
project(create_quic_draft_30_pcap C)

# Refer https://stackoverflow.com/a/26534464/2106207
# and https://cmake.org/cmake/help/latest/module/FindPkgConfig.html

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if(OPENSSL_FOUND)
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}, and libriries are ${OPENSSL_LIBRARIES}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by its own
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/")
find_package(PCAP REQUIRED)

add_executable(main main.c)
target_link_libraries(main ${OPENSSL_LIBRARIES} ${PCAP_LIBRARY} )
