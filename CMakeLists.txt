MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child} AND NOT ${child} MATCHES "test" AND EXISTS ${curdir}/${child}/CMakeLists.txt)
            LIST(APPEND dirlist "${child}")
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

cmake_minimum_required(VERSION 3.5)
project(vccc)

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

#enable_testing()
#add_subdirectory(test)

#target_include_directories(vccc_utility INTERFACE include)
