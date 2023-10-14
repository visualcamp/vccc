cmake_minimum_required(VERSION 3.5)
project(vccc_directory)

add_library(vccc_directory INTERFACE)
add_library(VCCC::directory ALIAS vccc_directory)

target_include_directories(vccc_directory INTERFACE include)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()

target_link_libraries(vccc INTERFACE vccc_directory)
