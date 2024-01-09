cmake_minimum_required(VERSION 3.5)
project(vccc_signal)

add_library(vccc_signal INTERFACE)
add_library(VCCC::signal ALIAS vccc_signal)

target_include_directories(vccc_signal INTERFACE include)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(my_app PRIVATE Threads::Threads)
else()
    target_link_libraries(vccc_signal INTERFACE VCCC)
endif()
