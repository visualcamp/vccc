cmake_minimum_required(VERSION 3.5)
project(vccc_type_traits)

add_library(vccc_type_traits INTERFACE)
add_library(VCCC::type_traits ALIAS vccc_type_traits)

target_include_directories(vccc_type_traits INTERFACE include)

if (MSVC)
    target_compile_options(vccc_type_traits INTERFACE /Zc:__cplusplus)
    target_compile_options(vccc_type_traits INTERFACE /Zc:ternary)
    target_compile_options(vccc_type_traits INTERFACE /permissive-)
endif ()

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
