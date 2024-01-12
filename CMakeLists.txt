cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(vccc)
add_library(vccc INTERFACE)
add_library(VCCC ALIAS vccc)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

include(Configuration)

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
    if (VCCC_COMPILER_SUPPORT_STD_14)
        VCCC_TEST_ONE_CXX(${name}_cxx14 ${target} 14 ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_17)
        VCCC_TEST_ONE_CXX(${name}_cxx17 ${target} 17 ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_20)
        VCCC_TEST_ONE_CXX(${name}_cxx20 ${target} 20 ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_23)
        VCCC_TEST_ONE_CXX(${name}_cxx23 ${target} 23 ${ARGN})
    endif ()
ENDMACRO()

MACRO(VCCC_TEST name target)
    VCCC_TEST_ALL_CXX(${name} ${target} ${ARGN})
ENDMACRO()

if(VCCC_RUN_TEST)
    if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
    endif()
    enable_testing()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test/include)
endif()

if (MSVC)
    target_compile_options(vccc INTERFACE /Zc:__cplusplus)
    target_compile_options(vccc INTERFACE /Zc:ternary)
    target_compile_options(vccc INTERFACE /permissive-)
endif ()


target_include_directories(vccc INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include/algorithm/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/concepts/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/core/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/functional/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/iterator/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/literal/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/memory/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/numeric/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/optional/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/ranges/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/signal/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/span/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/string_view/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/tuple/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/type_traits/include"
    "${CMAKE_CURRENT_LIST_DIR}/include/utility/include"
    "${VCCC_GENERATED_INCLUDE_DIR}"
)

add_subdirectory(include/directory)
add_subdirectory(include/log)
add_subdirectory(include/math)
add_subdirectory(include/random)
add_subdirectory(include/signal)
add_subdirectory(include/type_support)

target_link_libraries(vccc INTERFACE
    VCCC::directory
    VCCC::log
    VCCC::math
    VCCC::random
    VCCC::signal
    VCCC::type_support
)

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()
