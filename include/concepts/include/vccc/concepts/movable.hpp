//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVABLE_HPP_
#define VCCC_CONCEPTS_MOVABLE_HPP_

#include <type_traits>

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/swappable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_swappable.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value>
struct movable_impl : std::false_type {};

template<typename T>
struct movable_impl<T, true>
    : conjunction<
        std::is_object<T>,
        move_constructible<T>,
        assignable_from<T&, T>,
        swappable<T>
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename T>
struct movable : detail::movable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_MOVABLE_HPP_
