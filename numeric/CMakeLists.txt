cmake_minimum_required(VERSION 3.5)
project(vccc_numeric)

add_library(vccc_numeric INTERFACE)
add_library(VCCC::numeric ALIAS vccc_numeric)

target_include_directories(vccc_numeric INTERFACE include)

if(RUN_TEST)
    add_subdirectory(test)
endif()