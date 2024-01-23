project(vccc_type_support)

add_library(vccc_type_support INTERFACE)
add_library(VCCC::type_support ALIAS vccc_type_support)

target_include_directories(vccc_type_support INTERFACE include)
target_link_libraries(vccc_type_support INTERFACE VCCC)

if(VCCC_USE_OPENCV_FEATURES)
    target_compile_definitions(vccc_type_support INTERFACE VCCC_USE_OPENCV_FEATURES=1)
else()
    target_compile_definitions(vccc_type_support INTERFACE VCCC_USE_OPENCV_FEATURES=0)
endif()
