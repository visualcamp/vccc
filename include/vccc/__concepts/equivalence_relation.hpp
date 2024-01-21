//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_EQUIVALENCE_RELATION_HPP
#define VCCC_CONCEPTS_EQUIVALENCE_RELATION_HPP

#include "vccc/__concepts/relation.hpp"

namespace vccc {
/// @addtogroup concepts
/// @{

/**
@brief specifies that a relation imposes an equivalence relation

The concept `%equivalence_relation<R, T, U>` specifies that the `relation` `R` imposes an
[equivalence relation](https://en.wikipedia.org/wiki/Equivalence_relation) on its arguments.

@sa [std::equivalence_relation](https://en.cppreference.com/w/cpp/concepts/equivalence_relation)
 */
template<typename R, typename T, typename U>
struct equivalence_relation : relation<R, T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_EQUIVALENCE_RELATION_HPP
