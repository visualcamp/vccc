cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(vccc)
add_library(vccc INTERFACE)
add_library(VCCC ALIAS vccc)
add_library(vccc_stl INTERFACE)

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
    if (EMSCRIPTEN)
        target_link_libraries(${name} PUBLIC ${ARGN} -sEXIT_RUNTIME=1 -fexceptions)
    else()
        target_link_libraries(${name} PUBLIC ${ARGN})
    endif()
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

MACRO(VCCC_TEST_INCLUDE name type)
    if (VCCC_COMPILER_SUPPORT_STD_14)
        target_include_directories(${name}_cxx14 ${type} ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_17)
        target_include_directories(${name}_cxx17 ${type} ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_20)
        target_include_directories(${name}_cxx20 ${type} ${ARGN})
    endif ()
    if (VCCC_COMPILER_SUPPORT_STD_23)
        target_include_directories(${name}_cxx23 ${type} ${ARGN})
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
    target_compile_options(vccc_stl INTERFACE /Zc:__cplusplus)
    target_compile_options(vccc_stl INTERFACE /Zc:ternary)
    target_compile_options(vccc_stl INTERFACE /permissive-)
endif ()


target_include_directories(vccc_stl INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${VCCC_GENERATED_INCLUDE_DIR}"
)

add_subdirectory(include/directory)
add_subdirectory(include/literal)
add_subdirectory(include/log)
add_subdirectory(include/math)
add_subdirectory(include/random)
add_subdirectory(include/signal)
add_subdirectory(include/type_support)

target_link_libraries(vccc INTERFACE
    vccc_stl
    VCCC::directory
    VCCC::literal
    VCCC::log
    VCCC::math
    VCCC::random
    VCCC::signal
    VCCC::type_support
)

set_target_properties(vccc PROPERTIES PUBLIC_HEADER "${vccc_generated_headers}")

if(VCCC_RUN_TEST)
    add_subdirectory(test)
endif()


include(GNUInstallDirs)
install(TARGETS vccc PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vccc")
install(DIRECTORY include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

install(DIRECTORY include/directory/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/literal/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/log/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/math/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/random/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/signal/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/type_support/include/vccc DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

option(VCCC_INSTALL_BOOST "Install Boost.pfr and Boost.predef" ON)

if (VCCC_INSTALL_BOOST)
    install(DIRECTORY boost/pfr/include/boost DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY boost/predef/include/boost DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endif()
