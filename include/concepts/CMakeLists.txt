cmake_minimum_required(VERSION 3.5)
project(vccc_concepts)

add_library(vccc_concepts INTERFACE)
add_library(VCCC::concepts ALIAS vccc_concepts)

target_include_directories(vccc_concepts INTERFACE include)
target_link_libraries(vccc_concepts INTERFACE
    VCCC::functional
    VCCC::type_traits
    VCCC::utility)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
