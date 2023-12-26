//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_RELATION_HPP
#define VCCC_CONCEPTS_RELATION_HPP

#include "vccc/concepts/predicate.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
/// @addtogroup concepts
/// @{

/**
@brief specifies that a callable type is a binary relation

The concept `%relation<R, T, U>` specifies that R defines a binary relation over the set of expressions whose type and
value category are those encoded by either `T` or `U`.

@sa [std::relation](https://en.cppreference.com/w/cpp/concepts/relation)
 */
template<typename R, typename T, typename U>
struct relation
    : conjunction<
        predicate<R, T, T>,
        predicate<R, U, U>,
        predicate<R, T, U>,
        predicate<R, U, T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_RELATION_HPP
