cmake_minimum_required(VERSION 3.5)
project(vccc_tuple)

add_library(vccc_tuple INTERFACE)
add_library(VCCC::tuple ALIAS vccc_tuple)

target_include_directories(vccc_tuple INTERFACE include)
target_link_libraries(vccc_tuple INTERFACE VCCC::type_traits VCCC::functional)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()

