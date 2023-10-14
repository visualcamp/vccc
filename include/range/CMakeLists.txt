cmake_minimum_required(VERSION 3.5)
project(vccc_range)

add_library(vccc_range INTERFACE)
add_library(VCCC::range ALIAS vccc_range)

target_include_directories(vccc_range INTERFACE include)

target_link_libraries(vccc INTERFACE vccc_range)
