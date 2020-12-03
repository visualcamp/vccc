cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_utility VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_utility INTERFACE)
add_library(VCPP::utility ALIAS vcpp_utility)

target_include_directories(vcpp_utility INTERFACE include)
