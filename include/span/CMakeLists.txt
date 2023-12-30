cmake_minimum_required(VERSION 3.5)
project(vccc_span)

add_library(vccc_span INTERFACE)
add_library(VCCC::span ALIAS vccc_span)

target_include_directories(vccc_span INTERFACE include)
target_link_libraries(vccc_span INTERFACE VCCC::type_traits VCCC::functional VCCC::memory VCCC::ranges)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
