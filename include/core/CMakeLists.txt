cmake_minimum_required(VERSION 3.5)
project(vccc_core)

add_library(vccc_core INTERFACE)
add_library(VCCC::core ALIAS vccc_core)

target_include_directories(vccc_core INTERFACE include)

if (MSVC)
    target_compile_options(vccc_core INTERFACE /Zc:__cplusplus)
    target_compile_options(vccc_core INTERFACE /Zc:ternary)
    target_compile_options(vccc_core INTERFACE /permissive-)
endif ()

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
