cmake_minimum_required(VERSION 3.5)
project(vccc_signal)

add_library(vccc_signal INTERFACE)
add_library(VCCC::signal ALIAS vccc_signal)

target_include_directories(vccc_signal INTERFACE include)

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

target_link_libraries(vccc_signal INTERFACE VCCC::optional)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
