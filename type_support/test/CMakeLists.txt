set(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

macro(MAKE_TEST name target)
    add_executable(${name} ${target})
    target_link_libraries(${name} ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

if(VCCC_USE_OPENCV_FEATURES)
    find_package(OpenCV REQUIRED)
    MAKE_TEST(type_support_test ts_test.cpp
            VCCC::type_support
            VCCC::type_traits
            VCCC::utility
            ${OpenCV_LIBS}
            )
    target_include_directories(type_support_test PUBLIC ${OpenCV_INCLUDE_DIRS})
else()
    MAKE_TEST(type_support_test ts_test.cpp
            VCCC::type_support
            VCCC::type_traits
            VCCC::utility
            )
endif()
