cmake_minimum_required(VERSION 3.5)
project(vccc_random)

add_library(vccc_random INTERFACE)
add_library(VCCC::random ALIAS vccc_random)

target_include_directories(vccc_random INTERFACE include)

target_link_libraries(vccc INTERFACE vccc_random)
