cmake_minimum_required(VERSION 3.5)
project(vccc_directory)

add_library(vccc_directory INTERFACE)
add_library(VCCC::directory ALIAS vccc_directory)

target_include_directories(vccc_directory INTERFACE include)
