cmake_minimum_required(VERSION 3.5)
project(vccc_type_support)

add_library(vccc_type_support INTERFACE)
add_library(VCCC::type_support ALIAS vccc_type_support)

target_include_directories(vccc_type_support INTERFACE include)

set(VCCC_TYPE_SUPPORT_REQ_LIB VCCC::type_traits VCCC::utility)
set(VCCC_TYPE_SUPPORT_REQ_INC include)

if(VCCC_USE_OPENCV_FEATURES)
    find_package(OpenCV REQUIRED)
    LIST(APPEND VCCC_TYPE_SUPPORT_REQ_LIB ${OpenCV_LIBS})
    LIST(APPEND VCCC_TYPE_SUPPORT_REQ_INC ${OpenCV_INCLUDE_DIRS})
    set(VCCC_TYPE_SUPPORT_COMPILE_DEF "VCCC_USE_OPENCV_FEATURES=1")
else()
    set(VCCC_TYPE_SUPPORT_COMPILE_DEF "VCCC_USE_OPENCV_FEATURES=0")
endif()

target_include_directories(vccc_type_support INTERFACE ${VCCC_TYPE_SUPPORT_REQ_INC})
target_link_libraries(vccc_type_support INTERFACE ${VCCC_TYPE_SUPPORT_REQ_LIB})
target_compile_definitions(vccc_type_support INTERFACE ${VCCC_TYPE_SUPPORT_COMPILE_DEF})

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
