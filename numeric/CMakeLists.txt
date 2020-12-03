cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_numeric VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_numeric INTERFACE)
add_library(VCPP::numeric ALIAS vcpp_numeric)

target_include_directories(vcpp_numeric INTERFACE include)
