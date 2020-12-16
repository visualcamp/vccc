cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_high LANGUAGES CXX)

add_library(vccc_high INTERFACE)
add_library(VCCC::high ALIAS vccc_high)

target_include_directories(vccc_high INTERFACE include)
