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

option(RUN_TEST "Run Test in vccc" ON)

if(NOT DEFINED ${VCCC_USE_OPENCV_FEATURES})
    set(VCCC_USE_OPENCV_FEATURES ON)
endif()

add_library(vccc INTERFACE)
add_library(VCCC ALIAS vccc)
add_subdirectory(boost/predef)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

SET(libraries_list "")

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
    LIST(APPEND libraries_list "VCCC::${subdir}")
ENDFOREACH()

target_link_libraries(vccc INTERFACE ${libraries_list} Boost::predef)

if(RUN_TEST)
    enable_testing()
endif()
