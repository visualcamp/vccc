//
// Created by cosge on 2024-01-26.
//

#ifndef VCCC_ITERATOR_COMMON_ITERATOR_HPP_
#define VCCC_ITERATOR_COMMON_ITERATOR_HPP_

#include <type_traits>

#include "vccc/__concepts/copyable.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/negation.hpp"

namespace vccc {

// TODO: Implement variant
// template<typename I, typename S>
// class common_iterator {
//  public:
//   static_assert(input_or_output_iterator<I>::value, "Constraints not satisfied");
//   static_assert(sentinel_for<S, I>::value, "Constraints not satisfied");
//   static_assert(negation<same_as<I, S>>::value, "Constraints not satisfied");
//   static_assert(copyable<I>::value, "Constraints not satisfied");
//
//
//  private:
// };



} // namespace vccc

#endif // VCCC_ITERATOR_COMMON_ITERATOR_HPP_
