cmake_minimum_required(VERSION 3.5)

project(vccc_math LANGUAGES CXX)

add_library(vccc_math INTERFACE)
add_library(VCCC::math ALIAS vccc_math)

target_include_directories(vccc_math INTERFACE include)
