cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_range LANGUAGES CXX)

add_library(vccc_range INTERFACE)
add_library(VCCC::range ALIAS vccc_range)

target_include_directories(vccc_range INTERFACE include)
