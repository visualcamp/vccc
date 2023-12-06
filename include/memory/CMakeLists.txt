cmake_minimum_required(VERSION 3.5)
project(vccc_memory)

add_library(vccc_memory INTERFACE)
add_library(VCCC::memory ALIAS vccc_memory)

target_include_directories(vccc_memory INTERFACE include)
target_link_libraries(vccc_memory INTERFACE VCCC::type_traits)
