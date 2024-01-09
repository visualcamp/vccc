//
// Created by yonggyulee on 2024/01/08.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_COPYABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_COPYABLE_HPP

#include <type_traits>

#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/indirectly_writable.hpp"
#include "vccc/iterator/iter_reference_t.hpp"

namespace vccc {
namespace detail {

template<typename In, typename Out, bool = indirectly_readable<In>::value /* true */>
struct indirectly_copyable_impl
    : indirectly_writable<Out, iter_reference_t<In>> {};

template<typename In, typename Out>
struct indirectly_copyable_impl<In, Out, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that values may be copied from an `indirectly_readable` type to an `indirectly_writable` type

The `%indirectly_copyable` concept specifies the relationship between an `indirectly_readable` type and a type that is
`indirectly_writable`. The `indirectly_writable` type must be able to directly copy the object that the
`%indirectly_readable` type references.

@sa [std::indirectly_copyable](https://en.cppreference.com/w/cpp/iterator/indirectly_copyable)
 */
template<typename In, typename Out>
struct indirectly_copyable : detail::indirectly_copyable_impl<In, Out> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_COPYABLE_HPP
