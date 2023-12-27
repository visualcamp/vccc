cmake_minimum_required(VERSION 3.5)
project(vccc_algorithm)

add_library(vccc_algorithm INTERFACE)
add_library(VCCC::algorithm ALIAS vccc_algorithm)

target_include_directories(vccc_algorithm INTERFACE include)
target_link_libraries(vccc_algorithm INTERFACE
    VCCC::concepts
    VCCC::iterator
    VCCC::type_traits
)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
