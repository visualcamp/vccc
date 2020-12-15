cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_high_ver LANGUAGES CXX)

add_library(vccc_high_ver INTERFACE)
add_library(VCCC::high_ver ALIAS vccc_high_ver)

target_include_directories(vccc_high_ver INTERFACE include)
