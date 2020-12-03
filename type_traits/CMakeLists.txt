cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_type_traits VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_type_traits INTERFACE)
add_library(VCPP::type_traits ALIAS vcpp_type_traits)

target_include_directories(vcpp_type_traits INTERFACE include)
