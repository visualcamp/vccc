cmake_minimum_required(VERSION 3.5)
project(vccc_string_view)

add_library(vccc_string_view INTERFACE)
add_library(VCCC::string_view ALIAS vccc_string_view)

target_include_directories(vccc_string_view INTERFACE include)
target_link_libraries(vccc_string_view INTERFACE VCCC::type_traits VCCC::functional VCCC::memory)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
