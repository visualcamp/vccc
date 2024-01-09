cmake_minimum_required(VERSION 3.5)
project(vccc_ranges)

add_library(vccc_ranges INTERFACE)
add_library(VCCC::ranges ALIAS vccc_ranges)

target_include_directories(vccc_ranges INTERFACE include)
target_link_libraries(vccc_ranges INTERFACE
    VCCC::concepts
    VCCC::iterator
    VCCC::memory
    VCCC::optional
    VCCC::span
    VCCC::string_view
    VCCC::tuple
    VCCC::type_traits
)

if (MSVC)
    target_compile_options(vccc_ranges INTERFACE /Zc:__cplusplus)
    target_compile_options(vccc_ranges INTERFACE /Zc:ternary)
    target_compile_options(vccc_ranges INTERFACE /permissive-)
endif ()

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
