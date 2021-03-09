cmake_minimum_required(VERSION 3.5)
project(vccc_math)

add_library(vccc_math INTERFACE)
add_library(VCCC::math ALIAS vccc_math)

target_include_directories(vccc_math INTERFACE include)

if(RUN_TEST)
    add_subdirectory(test)
endif()