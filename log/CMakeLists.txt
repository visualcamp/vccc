cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_log VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_log INTERFACE)
add_library(VCPP::log ALIAS vcpp_log)

target_include_directories(vcpp_log INTERFACE include)
