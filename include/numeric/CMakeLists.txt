cmake_minimum_required(VERSION 3.5)
project(vccc_numeric)

add_library(vccc_numeric INTERFACE)
add_library(VCCC::numeric ALIAS vccc_numeric)

target_include_directories(vccc_numeric INTERFACE include)
target_link_libraries(vccc_numeric INTERFACE VCCC::type_traits VCCC::math)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()

target_link_libraries(vccc INTERFACE vccc_numeric)
