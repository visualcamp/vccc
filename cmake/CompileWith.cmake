include(CheckCXXStandardSupport)

function(CompileWith source out version)
    _GetCXXStandardCompilerFlagPrefix(_prefix)

    set(CMAKE_REQUIRED_FLAGS "${_prefix}${version}")
    check_cxx_source_compiles("${source}" ${out})
    set(${out} ${${out}} PARENT_SCOPE)
endfunction()
