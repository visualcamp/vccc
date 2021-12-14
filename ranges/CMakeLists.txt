cmake_minimum_required(VERSION 3.5)
project(vccc_ranges)

add_library(vccc_ranges INTERFACE)
add_library(VCCC::ranges ALIAS vccc_ranges)

target_include_directories(vccc_ranges INTERFACE include)
