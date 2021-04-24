set(CMAKE_CXX_STANDARD 14)

macro(MAKE_TEST name target)
    add_executable(${name} ${target})
    target_link_libraries(${name} ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

MAKE_TEST(access_test access_test.cpp VCCC::access)
