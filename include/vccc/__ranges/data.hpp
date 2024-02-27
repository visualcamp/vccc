//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_RANGES_DATA_HPP
#define VCCC_RANGES_DATA_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__core/decay_copy.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__memory/to_address.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__type_traits/detail/return_category.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

template<typename P>
struct data_member_check_2 : std::false_type {
  using category = return_category<0>;
};
template<typename P>
struct data_member_check_2<P*> : std::is_object<P> {
  using category = return_category<1, P*>;
};

template<typename T, typename = void>
struct data_member_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct data_member_check<T, void_t<decltype( vccc_decay_copy(std::declval<T>().data()) )>>
    : data_member_check_2<decltype( vccc_decay_copy(std::declval<T>().data()) )> {};


template<typename T, typename = void>
struct data_ranges_begin_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct data_ranges_begin_check<T, void_t<decltype( ranges::begin(std::declval<T>()) )>>
    : contiguous_iterator<decltype( ranges::begin(std::declval<T>()) )>
{
  using category = return_category<2, decltype( vccc::to_address( ranges::begin(std::declval<T>()) ) )>;
};

template<typename T>
struct data_category
    : std::conditional_t<
          data_member_check<T>::value, typename data_member_check<T>::category,
      std::conditional_t<
          data_ranges_begin_check<T>::value, typename data_ranges_begin_check<T>::category,
          return_category<0>
      >>{};

struct data_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T&&>,
      ranges::enable_borrowed_range<remove_cvref_t<T>>
  >::value, int> = 0>
  constexpr typename data_category<T&&>::return_type
  operator()(T&& t) const {
    return (*this)(std::forward<T>(t), data_category<T&&>{});
  }

 private:
  template<typename T, typename R>
  constexpr R operator()(T&& t, return_category<1, R>) const {
    return vccc_decay_copy(t.data());
  }

  template<typename T, typename R>
  constexpr R operator()(T&& t, return_category<2, R>) const {
    return vccc::to_address(ranges::begin(t));
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{


/**
@brief obtains a pointer to the beginning of a contiguous range

Returns a pointer to the first element of a contiguous range.

@sa [std::ranges::data](https://en.cppreference.com/w/cpp/ranges/data)
@sa ranges::cdata
@sa ranges::data
@sa vccc::data
 */
VCCC_INLINE_OR_STATIC constexpr detail::data_niebloid data{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_DATA_HPP
