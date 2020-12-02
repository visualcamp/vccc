# Copyright 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.16)

project(vcpp_type_support VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(vcpp_type_support INTERFACE)
add_library(VCPP::type_support ALIAS vcpp_type_support)

target_include_directories(vcpp_type_support INTERFACE include)
