cmake_minimum_required(VERSION 3.5)
project(vccc_type_support)

add_library(vccc_type_support INTERFACE)
add_library(VCCC::type_support ALIAS vccc_type_support)

target_include_directories(vccc_type_support INTERFACE include)
