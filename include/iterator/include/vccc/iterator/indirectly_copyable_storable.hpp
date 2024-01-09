//
// Created by yonggyulee on 2023/12/31.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_COPYABLE_STORABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_COPYABLE_STORABLE_HPP

#include <type_traits>

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/copyable.hpp"
#include "vccc/concepts/constructible_from.hpp"
#include "vccc/iterator/indirectly_copyable.hpp"
#include "vccc/iterator/indirectly_writable.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename In, typename Out, bool = indirectly_copyable<In, Out>::value /* true */>
struct indirectly_copyable_storable_impl
    : conjunction<
          indirectly_writable<Out, iter_value_t<In>&>,
          indirectly_writable<Out, const iter_value_t<In>&>,
          indirectly_writable<Out, iter_value_t<In>&&>,
          indirectly_writable<Out, const iter_value_t<In>&&>,
          copyable<iter_value_t<In>>,
          constructible_from<iter_value_t<In>, iter_reference_t<In>>,
          assignable_from<iter_value_t<In>&, iter_reference_t<In>>
      >{};

template<typename In, typename Out>
struct indirectly_copyable_storable_impl<In, Out, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that values may be copied from an `indirectly_readable` type to an `indirectly_writable` type and that
the copy may be performed via an intermediate object

The `%indirectly_copyable_storable` concept specifies the relationship between an `indirectly_readable` type and an
`indirectly_writable` type. In addition to `indirectly_copyable`, this concept specifies that the copy from the
`%indirectly_readable` type can be performed via an intermediate object.

@sa [std::indirectly_copyable_storable](https://en.cppreference.com/w/cpp/iterator/indirectly_copyable_storable)
 */
template<typename In, typename Out>
struct indirectly_copyable_storable : detail::indirectly_copyable_storable_impl<In, Out> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_COPYABLE_STORABLE_HPP
