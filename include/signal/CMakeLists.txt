cmake_minimum_required(VERSION 3.5)
project(vccc_signal)

add_library(vccc_signal INTERFACE)
add_library(VCCC::signal ALIAS vccc_signal)

target_include_directories(vccc_signal INTERFACE include)

target_link_libraries(vccc_signal INTERFACE VCCC)
if (UNIX AND NOT APPLE)
    target_compile_options(vccc_signal INTERFACE -fopenmp)
endif()
