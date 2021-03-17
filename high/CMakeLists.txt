cmake_minimum_required(VERSION 3.5)
project(vccc_high)

add_library(vccc_high INTERFACE)
add_library(VCCC::high ALIAS vccc_high)

target_include_directories(vccc_high INTERFACE include)

if(RUN_TEST)
    add_subdirectory(test)
endif()