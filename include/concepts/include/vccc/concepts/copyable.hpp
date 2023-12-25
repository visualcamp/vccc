//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPYABLE_HPP_
#define VCCC_CONCEPTS_COPYABLE_HPP_

#include <type_traits>

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace concepts {
namespace detail {

template<typename T, bool = is_referencable<T>::value>
struct copyable_impl : std::false_type {};

template<typename T>
struct copyable_impl<T, true>
    : conjunction<
        copy_constructible<T>,
        movable<T>,
        assignable_from<T&, T&>,
        assignable_from<T&, const T&>,
        assignable_from<T&, const T>
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename T>
struct copyable : detail::copyable_impl<T> {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_COPYABLE_HPP_
