cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_literal VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_literal INTERFACE)
add_library(VCPP::literal ALIAS vcpp_literal)

target_include_directories(vcpp_literal INTERFACE include)
