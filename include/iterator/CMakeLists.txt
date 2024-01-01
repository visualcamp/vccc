cmake_minimum_required(VERSION 3.5)
project(vccc_iterator)

add_library(vccc_iterator INTERFACE)
add_library(VCCC::iterator ALIAS vccc_iterator)

target_include_directories(vccc_iterator INTERFACE include)
target_link_libraries(vccc_iterator INTERFACE
    VCCC::core
    VCCC::type_traits
    VCCC::concepts
    VCCC::memory
    VCCC::ranges # TODO: Remove circular dependency
    )

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
