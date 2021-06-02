cmake_minimum_required(VERSION 3.5)
project(vccc_experimental)

add_library(vccc_experimental INTERFACE)
add_library(VCCC::experimental ALIAS vccc_experimental)

target_include_directories(vccc_experimental INTERFACE include)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(vccc_experimental INTERFACE Threads::Threads)
endif()

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()