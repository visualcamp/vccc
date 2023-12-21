cmake_minimum_required(VERSION 3.5)
project(vccc_optional)

add_library(vccc_optional INTERFACE)
add_library(VCCC::optional ALIAS vccc_optional)

target_link_libraries(vccc_optional INTERFACE VCCC::type_traits)
target_include_directories(vccc_optional INTERFACE include)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
