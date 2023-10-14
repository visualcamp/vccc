cmake_minimum_required(VERSION 3.5)
project(vccc_literal)

add_library(vccc_literal INTERFACE)
add_library(VCCC::literal ALIAS vccc_literal)

target_include_directories(vccc_literal INTERFACE include)

target_link_libraries(vccc INTERFACE vccc_literal)
