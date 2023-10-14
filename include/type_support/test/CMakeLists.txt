option(VCCC_TEST_SYSTEM_OPENCV "Use system installed OpenCV on test" OFF)

if(VCCC_TEST_SYSTEM_OPENCV)
    find_package(OpenCV REQUIRED)
    VCCC_TEST(type_support_test ts_test.cpp VCCC::type_support ${OpenCV_LIBS})
    VCCC_TEST_INCLUDE(type_support_test PUBLIC ${OpenCV_INCLUDE_DIRS})
else()
    VCCC_TEST(type_support_test ts_test.cpp VCCC::type_support)
endif()
