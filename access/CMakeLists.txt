cmake_minimum_required(VERSION 3.5)
project(vccc_access)

add_library(vccc_access INTERFACE)
add_library(VCCC::access ALIAS vccc_access)

target_include_directories(vccc_access INTERFACE include)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()