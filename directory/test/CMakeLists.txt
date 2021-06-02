set(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

macro(MAKE_TEST name target)
    add_executable(${name} ${target})
    target_link_libraries(${name} ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

MAKE_TEST(directory_test directory_test.cc VCCC::directory)
