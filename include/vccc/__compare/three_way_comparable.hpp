//
// Created by YongGyu Lee on 4/11/24.
//

#ifndef VCCC_COMPARE_THREE_WAY_COMPARABLE_HPP_
#define VCCC_COMPARE_THREE_WAY_COMPARABLE_HPP_

#include "vccc/__concepts/partially_ordered_with.hpp"
#include "vccc/__concepts/weakly_equality_comparable_with.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

template<typename T>
struct unstable_three_way_comparable
    : conjunction<
        weakly_equality_comparable_with<T, T>,
        partially_ordered_with<T, T>
    > {};

} // namespace vccc

#endif // VCCC_COMPARE_THREE_WAY_COMPARABLE_HPP_
