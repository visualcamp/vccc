cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_log VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vccc_log INTERFACE)
add_library(VCCC::log ALIAS vccc_log)

target_include_directories(vccc_log INTERFACE include)
