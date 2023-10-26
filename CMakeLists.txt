cmake_minimum_required(VERSION 3.5)

project(vccc)
add_library(vccc INTERFACE)
add_library(VCCC ALIAS vccc)

# Third party
add_subdirectory(boost/predef)
add_subdirectory(boost/pfr)

# Options
option(VCCC_RUN_TEST "Run Test in vccc" OFF)
option(VCCC_USE_OPENCV_FEATURES "Use OpenCV" ON)

message("VCCC_RUN_TEST: ${VCCC_RUN_TEST}")
message("VCCC_USE_OPENCV_FEATURES: ${VCCC_USE_OPENCV_FEATURES}")

# TEST
MACRO(VCCC_TEST_ONE name target)
    add_test(NAME ${name} COMMAND ${name})
    add_executable(${name} ${target})
    target_link_libraries(${name} PUBLIC ${ARGN})
ENDMACRO()

MACRO(VCCC_TEST_ONE_CXX name target cxx)
    VCCC_TEST_ONE(${name} ${target} ${ARGN})
    set_property(TARGET ${name} PROPERTY CXX_STANDARD ${cxx})
ENDMACRO()

MACRO(VCCC_TEST_ALL_CXX name target)
#    VCCC_TEST_ONE_CXX(${name}_cxx11 ${target} 11 ${ARGN})
    VCCC_TEST_ONE_CXX(${name}_cxx14 ${target} 14 ${ARGN})
    VCCC_TEST_ONE_CXX(${name}_cxx17 ${target} 17 ${ARGN})
#    VCCC_TEST_ONE_CXX(${name}_cxx20 ${target} 20 ${ARGN}) compiler not ready on our machines
ENDMACRO()

MACRO(VCCC_TEST name target)
    if(VCCC_TEST_MATRIX)
        VCCC_TEST_ONE(${name} ${target} ${ARGN})
    else()
        VCCC_TEST_ALL_CXX(${name} ${target} ${ARGN})
    endif()
ENDMACRO()

MACRO(VCCC_TEST_INCLUDE name type)
    if(VCCC_TEST_MATRIX)
        target_include_directories(${name} ${type} ${ARGN})
    else()
        target_include_directories(${name}_cxx14 ${type} ${ARGN})
        target_include_directories(${name}_cxx17 ${type} ${ARGN})
    endif()
ENDMACRO()

if(VCCC_RUN_TEST)
    if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
    endif()
    enable_testing()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test/include)
endif()


# add modules
MACRO(AddModules dir)
    FILE(GLOB children RELATIVE ${dir} ${dir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${dir}/${child} AND EXISTS ${dir}/${child}/CMakeLists.txt)
            add_subdirectory("${dir}/${child}")
            target_link_libraries(vccc INTERFACE VCCC::${child})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

AddModules("${CMAKE_CURRENT_SOURCE_DIR}/include")
