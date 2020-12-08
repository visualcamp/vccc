cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_random LANGUAGES CXX)

add_library(vccc_random INTERFACE)
add_library(VCCC::random ALIAS vccc_random)

target_include_directories(vccc_random INTERFACE include)
