cmake_minimum_required(VERSION 3.5)
project(vccc_math)

add_library(vccc_math INTERFACE)
add_library(VCCC::math ALIAS vccc_math)

target_include_directories(vccc_math INTERFACE include)
target_link_libraries(vccc_math INTERFACE VCCC::type_traits)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()