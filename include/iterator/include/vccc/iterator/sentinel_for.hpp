//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_SENTINEL_FOR_HPP_
#define VCCC_ITERATOR_SENTINEL_FOR_HPP_

#include "vccc/concepts/semiregular.hpp"
#include "vccc/concepts/weakly_equality_comparable_with.hpp"
#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

template<typename S, typename I>
struct sentinel_for
    : conjunction<
        semiregular<S>,
        input_or_output_iterator<I>,
        weakly_equality_comparable_with<S, I>
      > {};

} // namespace vccc

#endif // VCCC_ITERATOR_SENTINEL_FOR_HPP_
