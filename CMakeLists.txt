MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child} AND EXISTS ${curdir}/${child}/CMakeLists.txt)
            LIST(APPEND dirlist "${child}")
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

cmake_minimum_required(VERSION 3.5)
project(vccc)

if(NOT DEFINED ${VCCC_RUN_TEST})
    option(VCCC_RUN_TEST "Run Test in vccc" OFF)
endif()

if(NOT DEFINED ${VCCC_USE_OPENCV_FEATURES})
    option(VCCC_USE_OPENCV_FEATURES "Use OpenCV" ON)
endif()

if(VCCC_RUN_TEST)
    enable_testing()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test/include)
endif()

add_library(vccc INTERFACE)
add_library(VCCC ALIAS vccc)
add_subdirectory(boost/predef)
add_subdirectory(boost/pfr)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

SET(libraries_list "")

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
    LIST(APPEND libraries_list "VCCC::${subdir}")
ENDFOREACH()
target_link_libraries(vccc INTERFACE ${libraries_list} Boost::predef Boost::pfr)


