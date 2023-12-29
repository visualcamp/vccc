cmake_minimum_required(VERSION 3.5)
project(vccc_functional)

add_library(vccc_functional INTERFACE)
add_library(VCCC::functional ALIAS vccc_functional)

target_include_directories(vccc_functional INTERFACE include)
target_link_libraries(vccc_functional INTERFACE VCCC::type_traits)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
