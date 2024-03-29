include(CheckCXXStandardSupport)
include(CheckCXX_TypeExists)
include(CheckConstexprAddressof)

if ("${CMAKE_CXX_STANDARD}" STREQUAL "")
    CheckCXXStandardSupport(14 VCCC_COMPILER_SUPPORT_STD_14)
    CheckCXXStandardSupport(17 VCCC_COMPILER_SUPPORT_STD_17)
    CheckCXXStandardSupport(20 VCCC_COMPILER_SUPPORT_STD_20)
    CheckCXXStandardSupport(23 VCCC_COMPILER_SUPPORT_STD_23)
else()
    set(VCCC_COMPILER_SUPPORT_STD_14 0)
    set(VCCC_COMPILER_SUPPORT_STD_17 0)
    set(VCCC_COMPILER_SUPPORT_STD_20 0)
    set(VCCC_COMPILER_SUPPORT_STD_23 0)
    set(VCCC_COMPILER_SUPPORT_STD_${CMAKE_CXX_STANDARD} 1)
endif ()

if(VCCC_COMPILER_SUPPORT_STD_23)
    set(VCCC_COMPILER_SUPPORT_STD 23)
elseif(VCCC_COMPILER_SUPPORT_STD_20)
    set(VCCC_COMPILER_SUPPORT_STD 20)
elseif(VCCC_COMPILER_SUPPORT_STD_17)
    set(VCCC_COMPILER_SUPPORT_STD 17)
elseif(VCCC_COMPILER_SUPPORT_STD_14)
    set(VCCC_COMPILER_SUPPORT_STD 14)
else()
    message(FATAL_ERROR "Compiler must support C++14")
endif ()

# Generate config header
set(VCCC_GENERATED_DIR "${PROJECT_BINARY_DIR}/generated")
set(VCCC_GENERATED_INCLUDE_DIR "${VCCC_GENERATED_DIR}/include")

if (NOT EXISTS "${VCCC_GENERATED_INCLUDE_DIR}/vccc")
    file(MAKE_DIRECTORY "${VCCC_GENERATED_INCLUDE_DIR}/vccc")
endif ()

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/__config.cmake.in"
    "${VCCC_GENERATED_INCLUDE_DIR}/vccc/__config.h"
)

list(APPEND vccc_generated_headers "${VCCC_GENERATED_INCLUDE_DIR}/vccc/__config.h")
