//
// Created by yonggyulee on 2024/02/08.
//

#ifndef VCCC_RANGES_REND_HPP_
#define VCCC_RANGES_REND_HPP_

#include <iterator>
#include <type_traits>

#include "vccc/__core/decay_copy.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/detail/not_incomplete_array.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/rbegin.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::tag_1;
using vccc::detail::tag_2;
using vccc::detail::tag_3;

struct rend_niebloid {
 private:

  template<typename T, bool = is_invocable<rbegin_niebloid, T&&>::value, typename = void>
  struct rend_member_check : std::false_type {};
  template<typename T>
  struct rend_member_check<T,
        true, void_t<decltype(vccc_decay_copy( std::declval<T>().rend() ))>>
      : sentinel_for<decltype(vccc_decay_copy( std::declval<T>().rend() )),
                     decltype( ranges::rbegin(std::declval<T>()) )> {};

  template<typename T, bool = conjunction<
      is_invocable<rbegin_niebloid, T&&>,
      is_class_or_enum<remove_cvref_t<T>>>::value, typename = void>
  struct rend_global_check : std::false_type {};
  template<typename T>
  struct rend_global_check<T,
        true, void_t<decltype(vccc_decay_copy( rend(std::declval<T>()) ))>>
      : sentinel_for<decltype(vccc_decay_copy( rend(std::declval<T>()) )),
                     decltype( ranges::rbegin(std::declval<T>()) )> {};

  template<typename T, bool = common_range<T>::value>
  struct common_bidi_check : std::false_type {};
  template<typename T>
  struct common_bidi_check<T, true> : bidirectional_iterator<decltype( ranges::begin(std::declval<T>()) )> {};

  template<typename T>
  using rend_tag = conditional_tag<rend_member_check<T>, rend_global_check<T>, common_bidi_check<T>>;

  template<typename T>
  constexpr auto run(T&& t, tag_1) const {
    return vccc_decay_copy(t.rend());
  }

  template<typename T>
  constexpr auto run(T&& t, tag_2) const {
    return vccc_decay_copy(rend(t));
  }

  template<typename T>
  constexpr auto run(T&& t, tag_3) const {
    return std::make_reverse_iterator(ranges::begin(t));
  }

 public:
  template<typename T, std::enable_if_t<conjunction<
      vccc::detail::not_incomplete_array<T>,
      disjunction<
        std::is_lvalue_reference<std::remove_cv_t<T>>,
        enable_borrowed_range<std::remove_cv_t<T>>
      >,
      bool_constant<(rend_tag<T>::value > 0)>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    return run(std::forward<T>(t), rend_tag<T>{});
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::rend_niebloid rend{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_REND_HPP_
