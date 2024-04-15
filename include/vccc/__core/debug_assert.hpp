//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_CORE_DEBUG_ASSERT_HPP_
#define VCCC_CORE_DEBUG_ASSERT_HPP_

#include <cassert>

#define VCCC_LINE_LOCATION_IMPL2(file, line) \
    "file: " file ", line " # line

#define VCCC_LINE_LOCATION_IMPL(file, line) \
    VCCC_LINE_LOCATION_IMPL2(file, line)

#define VCCC_LINE_LOCATION \
    VCCC_LINE_LOCATION_IMPL(__FILE__, __LINE__)

#define VCCC_DEBUG_ASSERT(expr) \
    assert(((void) "Assertion on " VCCC_LINE_LOCATION, expr))

#endif // VCCC_CORE_DEBUG_ASSERT_HPP_
