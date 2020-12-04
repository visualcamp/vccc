cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_numeric VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vccc_numeric INTERFACE)
add_library(VCCC::numeric ALIAS vccc_numeric)

target_include_directories(vccc_numeric INTERFACE include)
