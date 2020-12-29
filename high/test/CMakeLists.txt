CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(VCCC_TEST)


set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type -O3")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/vccc/high)

add_executable(VCCC_TEST main.cpp)

target_link_libraries(VCCC_TEST PUBLIC VCCC::high)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)