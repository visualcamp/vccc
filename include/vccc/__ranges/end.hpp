//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_END_HPP_
#define VCCC_RANGES_END_HPP_

#include <cstddef>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__type_traits/detail/return_category.hpp"
#include "vccc/__core/decay_copy.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_bounded_array.hpp"
#include "vccc/__type_traits/is_complete.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

struct end_niebloid {
 private:
  template<typename T, bool = is_bounded_array<remove_cvref_t<T>>::value>
  struct end_array_check : std::false_type {
    using category = return_category<0>;
  };
  template<typename T>
  struct end_array_check<T, true> : std::true_type {
    using category = return_category<1, decltype(std::declval<T>() + std::extent<std::remove_reference_t<T>>::value)>;
  };

  template<typename T, typename = void>
  struct end_member_check : std::false_type {
    using category = return_category<0>;
  };
  template<typename T>
  struct end_member_check<T, void_t<decltype(vccc_decay_copy(std::declval<T>().end()))>>
      : sentinel_for<decltype(vccc_decay_copy(std::declval<T>().end())), iterator_t<T>> {
    using category = return_category<2, decltype(vccc_decay_copy(std::declval<T>().end()))>;
  };

  template<typename T, typename = void>
  struct end_global_check : std::false_type {
    using category = return_category<0>;
  };
  template<typename T>
  struct end_global_check<T, void_t<decltype(vccc_decay_copy(end(std::declval<T>())))>>
      : sentinel_for<decltype(vccc_decay_copy(end(std::declval<T>()))), iterator_t<T>> {
    using category = return_category<3, decltype(vccc_decay_copy(end(std::declval<T>())))>;
  };

  template<typename T, bool = end_member_check<T>::value /* false */>
  struct end_category_impl_2 : end_global_check<T> {};
  template<typename T>
  struct end_category_impl_2<T, true> : end_member_check<T> {};

  template<typename T, bool = end_array_check<T>::value /* false */>
  struct end_category_impl : end_category_impl_2<T> {};
  template<typename T>
  struct end_category_impl<T, true> : end_array_check<T> {};

  template<typename T>
  struct end_category
      : std::conditional_t<
          disjunction<
            std::is_lvalue_reference<T>,
            enable_borrowed_range<remove_cvref_t<T>>
          >::value,
          typename end_category_impl<T>::category,
          return_category<0>
        > {};

  template<typename T, typename R>
  constexpr R operator()(T&& t, return_category<1, R>) const {
    static_assert(is_complete<std::remove_all_extents_t<std::remove_reference_t<T>>>::value, "Array element must be complete type");
    return t + std::extent<remove_cvref_t<T>>::value;
  }

  template<typename T, typename R>
  constexpr R operator()(T&& t, return_category<2, R>) const {
    return vccc_decay_copy(t.end());
  }

  template<typename T, typename R>
  constexpr R operator()(T&& t, return_category<3, R>) const {
    return vccc_decay_copy(end(t));
  }

 public:
  template<typename T>
  constexpr typename end_category<T&&>::return_type
  operator()(T&& t) const {
    return (*this)(std::forward<T>(t), end_category<T&&>{});
  }
};

} // namespace detail

namespace niebloid {

/// @addtogroup ranges
/// @{

/**
@brief returns a sentinel indicating the end of a range

Returns a sentinel indicating the end of a range.

@sa [std::ranges::end](https://en.cppreference.com/w/cpp/ranges/end])
 */

VCCC_INLINE_OR_STATIC constexpr detail::end_niebloid end{};

/// @}

} // namespace niebloid
using namespace niebloid;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_END_HPP_
