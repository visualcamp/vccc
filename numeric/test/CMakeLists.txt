set(CMAKE_CXX_STANDARD 14)

macro(MAKE_TEST name target)
    add_executable(${name} ${target})
    target_link_libraries(${name} ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

MAKE_TEST(numeric_test numeric_test.cpp VCCC::numeric VCCC::type_traits)
