cmake_minimum_required(VERSION 3.5)
project(vccc_log)

add_library(vccc_log INTERFACE)
add_library(VCCC::log ALIAS vccc_log)

target_include_directories(vccc_log INTERFACE include)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()