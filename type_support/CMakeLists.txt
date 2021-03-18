cmake_minimum_required(VERSION 3.5)
project(vccc_type_support)

add_library(vccc_type_support INTERFACE)
add_library(VCCC::type_support ALIAS vccc_type_support)

target_include_directories(vccc_type_support INTERFACE include)

if(VCCC_USE_OPENCV_FEATURES)
    add_definitions(-DVCCC_USE_OPENCV_FEATURES=1)
else()
    add_definitions(-DVCCC_USE_OPENCV_FEATURES=0)
endif()

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
