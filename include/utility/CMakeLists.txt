cmake_minimum_required(VERSION 3.5)
project(vccc_utility)

add_library(vccc_utility INTERFACE)
add_library(VCCC::utility ALIAS vccc_utility)

target_include_directories(vccc_utility INTERFACE include)
target_link_libraries(vccc_utility INTERFACE
    VCCC::core
    VCCC::type_traits)
