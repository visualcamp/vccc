<<<<<<< Updated upstream
CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(VCCC_TEST)


set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type -O3")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/vccc/high)

add_executable(VCCC_TEST main.cpp)

target_link_libraries(VCCC_TEST PUBLIC VCCC::high)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
=======
set(CMAKE_CXX_STANDARD 14)

macro(MAKE_TEST name target)
    add_executable(${name} ${target})
    target_link_libraries(${name} ${ARGN})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

MAKE_TEST(high_functional functional_test.cpp VCCC::high)
>>>>>>> Stashed changes
