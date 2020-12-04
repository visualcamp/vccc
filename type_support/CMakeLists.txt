cmake_minimum_required(VERSION 3.5...3.16)

project(vccc_type_support VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vccc_type_support INTERFACE)
add_library(VCCC::type_support ALIAS vccc_type_support)

target_include_directories(vccc_type_support INTERFACE include)
