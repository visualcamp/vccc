cmake_minimum_required(VERSION 3.5)
project(vccc_log)

add_library(vccc_log INTERFACE)
add_library(VCCC::log ALIAS vccc_log)

target_include_directories(vccc_log INTERFACE include)

if(ANDROID)
    find_library(log-lib log REQUIRED)
    target_link_libraries(vccc_log INTERFACE vccc_stl Boost::predef Boost::pfr ${log-lib})
else()
    target_link_libraries(vccc_log INTERFACE vccc_stl Boost::predef Boost::pfr)
endif()
