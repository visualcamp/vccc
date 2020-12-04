cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_utility VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vccc_utility INTERFACE)
add_library(VCCC::utility ALIAS vccc_utility)

target_include_directories(vccc_utility INTERFACE include)
