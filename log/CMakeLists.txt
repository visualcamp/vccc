cmake_minimum_required(VERSION 3.5)
project(vccc_log)

add_library(vccc_log INTERFACE)
add_library(VCCC::log ALIAS vccc_log)

target_include_directories(vccc_log INTERFACE include)
target_link_libraries(vccc_log INTERFACE VCCC::type_traits Boost::predef Boost::pfr)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()