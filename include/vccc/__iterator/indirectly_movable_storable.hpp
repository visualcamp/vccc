//
// Created by yonggyulee on 2024/01/10.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_MOVABLE_STORABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_MOVABLE_STORABLE_HPP

#include <type_traits>

#include "vccc/__concepts/assignable_from.hpp"
#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/movable.hpp"
#include "vccc/__iterator/indirectly_movable.hpp"
#include "vccc/__iterator/indirectly_writable.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename In, typename Out, bool = indirectly_movable<In, Out>::value /* true */>
struct indirectly_movable_storable_impl
    : conjunction<
          indirectly_writable<Out, iter_value_t<In>>,
          movable<iter_value_t<In>>,
          constructible_from<iter_value_t<In>, iter_rvalue_reference_t<In>>,
          assignable_from<iter_value_t<In>&, iter_rvalue_reference_t<In>>
      >{};

template<typename In, typename Out>
struct indirectly_movable_storable_impl<In, Out, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that values may be moved from an `indirectly_readable` type to an `indirectly_writable` type and that
the move may be performed via an intermediate object

The `%indirectly_movable_storable` concept specifies the relationship between an `indirectly_readable` type and an
`indirectly_writable` type. In addition to `indirectly_movable`, this concept specifies that the move from the
`indirectly_readable` type can be performed via an intermediate object.

@sa [std::indirectly_movable_storable](https://en.cppreference.com/w/cpp/iterator/indirectly_movable_storable)
 */
template<typename In, typename Out>
struct indirectly_movable_storable : detail::indirectly_movable_storable_impl<In, Out> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_MOVABLE_STORABLE_HPP
