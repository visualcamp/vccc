cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_literal VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vccc_literal INTERFACE)
add_library(VCCC::literal ALIAS vccc_literal)

target_include_directories(vccc_literal INTERFACE include)
