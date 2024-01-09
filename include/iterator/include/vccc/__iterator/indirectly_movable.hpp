//
// Created by yonggyulee on 2024/01/10.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_MOVABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_MOVABLE_HPP

#include <type_traits>

#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/indirectly_writable.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"

namespace vccc {
namespace detail {

template<typename In, typename Out, bool = indirectly_readable<In>::value /* true */>
struct indirectly_movable_impl
    : indirectly_writable<Out, iter_rvalue_reference_t<In>> {};

template<typename In, typename Out>
struct indirectly_movable_impl<In, Out, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that values may be moved from an `indirectly_readable` type to an `indirectly_writable` type

The `%indirectly_movable` concept specifies the relationship between an `indirectly_readable` type and a type that is
`indirectly_writable`. The `%indirectly_writable` type must be able to directly move the object that the
`%indirectly_readable` type references.

@sa [std::indirectly_movable](https://en.cppreference.com/w/cpp/iterator/indirectly_movable)
 */
template<typename In, typename Out>
struct indirectly_movable : detail::indirectly_movable_impl<In, Out> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_MOVABLE_HPP
