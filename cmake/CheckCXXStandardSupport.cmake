include(CheckCXXSourceCompiles)
include(CheckCXXCompilerFlag)

function(_GetCXXStandardCompilerFlagPrefix out_var)
    if(MSVC)
        set(${out_var} "/std:c++" PARENT_SCOPE)
    else()
        set(${out_var} "-std=c++" PARENT_SCOPE)
    endif()
endfunction()

_GetCXXStandardCompilerFlagPrefix(_flag_prefix)
set(__VCCC_STD_CXX_COMPILER_FLAG_PREFIX "${_flag_prefix}" CACHE STRING "Prefix compiler flag for C++ standard")

check_cxx_compiler_flag("${__VCCC_STD_CXX_COMPILER_FLAG_PREFIX}14" __VCCC_COMPILE_TEST_STD_CXX_14)
check_cxx_compiler_flag("${__VCCC_STD_CXX_COMPILER_FLAG_PREFIX}17" __VCCC_COMPILE_TEST_STD_CXX_17)
check_cxx_compiler_flag("${__VCCC_STD_CXX_COMPILER_FLAG_PREFIX}20" __VCCC_COMPILE_TEST_STD_CXX_20)
check_cxx_compiler_flag("${__VCCC_STD_CXX_COMPILER_FLAG_PREFIX}23" __VCCC_COMPILE_TEST_STD_CXX_23)

function(GetCXXStandardCompilerFlag version out_var)
    if ("${__VCCC_COMPILE_TEST_STD_CXX_${version}}")
        set(${out_var} "${__VCCC_STD_CXX_COMPILER_FLAG_PREFIX}${version}" PARENT_SCOPE)
        return()
    endif ()

    set(${out_var} "NOTFOUND" PARENT_SCOPE)
endfunction(GetCXXStandardCompilerFlag)

function(CheckCXXStandardSupport version variable)
    if (NOT "${version}" MATCHES "^(14|17|20|23)$")
        message(FATAL_ERROR "Unsupported C++ standard: ${version}")
    endif()

    GetCXXStandardCompilerFlag(${version} _flag)
    if ("${_flag}" STREQUAL "NOTFOUND")
        set(${variable} 0 PARENT_SCOPE)
        return()
    endif()

    set(_cplusplus_14 "201402L")
    set(_cplusplus_17 "201703L")
    set(_cplusplus_20 "202002L")
    set(_cplusplus_23 "202302L")

    set(CMAKE_REQUIRED_FLAGS "${_flag}")
    if (MSVC)
        set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} /Zc:__cplusplus")
    endif()
    set(_source "
#if __cplusplus < ${_cplusplus_${version}}
#error 1
#endif
int main() { return 0; }")
    check_cxx_source_compiles("${_source}" __VCCC_COMPILE_TEST_FULL_CXX_${version}_SUPPORT)
    set(${variable} ${__VCCC_COMPILE_TEST_FULL_CXX_${version}_SUPPORT} PARENT_SCOPE)
endfunction()



