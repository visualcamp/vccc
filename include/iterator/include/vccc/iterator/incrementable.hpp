//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_INCREMENTABLE_HPP_
#define VCCC_ITERATOR_INCREMENTABLE_HPP_

#include "vccc/concepts/regular.hpp"
#include "vccc/iterator/weakly_incrementable.hpp"

namespace vccc {
namespace concepts {

/// @addtogroup iterator
/// @{

template<typename T>
struct incrementable
    : conjunction<
        regular<T>,
        weakly_incrementable<T>
      > {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_ITERATOR_INCREMENTABLE_HPP_
