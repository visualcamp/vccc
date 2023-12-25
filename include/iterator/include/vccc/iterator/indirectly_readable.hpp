//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_READABLE_HPP_
#define VCCC_ITERATOR_INDIRECTLY_READABLE_HPP_

#include <type_traits>

#include "vccc/concepts/common_reference_with.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/iterator/indirectly_readable_traits.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_rvalue_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/type_traits/detail/requires_helper.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {
namespace concepts {
namespace detail {

template<typename In, bool = is_referencable<In>::value>
struct indirectly_readable_impl : std::false_type {};

template<typename In>
struct indirectly_readable_impl<In, true>
    : conjunction<
        concepts::common_reference_with<iter_reference_t<In>&&, iter_value_t<In>&>,
        concepts::common_reference_with<iter_reference_t<In>&&, iter_rvalue_reference_t<In>&&>,
        concepts::common_reference_with<iter_rvalue_reference_t<In>&&, const iter_value_t<In>&>
      >{};

template<
  typename In,
  bool = conjunction<
        concepts::dereferenceable<In>,
        vccc::detail::test_iter_value<In>,
        vccc::detail::require<iter_reference<In>>,
        vccc::detail::require<iter_rvalue_reference<In>>
      >::value
>
struct indirectly_readable_requires : std::false_type {};

template<typename In>
struct indirectly_readable_requires<In, true>
    : conjunction<
        concepts::same_as<decltype(*std::declval<const In&>()), iter_reference_t<In>>,
        concepts::same_as<decltype(ranges::iter_move(std::declval<const In&>())), iter_rvalue_reference_t<In>>,
        indirectly_readable_impl<In>
      > {};

} // namespace detail

template<typename In>
struct indirectly_readable : detail::indirectly_readable_requires<remove_cvref_t<In>> {};

} // namespace concepts
} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_READABLE_HPP_