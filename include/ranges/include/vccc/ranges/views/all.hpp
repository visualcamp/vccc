//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_RANGES_VIEWS_ALL_HPP
#define VCCC_RANGES_VIEWS_ALL_HPP

#include <type_traits>
#include <utility>

#include "vccc/core/inline_or_static.hpp"
#include "vccc/ranges/owning_view.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/ref_view.hpp"
#include "vccc/ranges/view.hpp"
#include "vccc/ranges/viewable_range.hpp"
#include "vccc/type_traits/detail/return_category.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

using vccc::detail::return_category;

struct all_niebloid {
 private:
  template<typename T>
  using return_category_type =
      std::conditional_t<
          view<std::decay_t<T>>::value, return_category<1, std::decay_t<T>>,
      std::conditional_t<
          conjunction<
              range<std::remove_reference_t<T>>,
              std::is_object<std::remove_reference_t<T>>,
              std::is_lvalue_reference<T>
          >::value,
              return_category<2, ref_view<std::remove_reference_t<T>>>,
              return_category<3, owning_view<std::remove_reference_t<T>>>
      >>;

  template<typename R, typename T>
  constexpr T operator()(R&& r, return_category<1, T>) const {
    return std::forward<R>(r);
  }

  template<typename R, typename T>
  constexpr T operator()(R&& r, return_category<2, T>) const {
    return {std::forward<R>(r)}; // ref_view
  }

  template<typename R, typename T>
  constexpr T operator()(R&& r, return_category<3, T>) const {
    return {std::forward<R>(r)}; // owning_view
  }

 public:
  template<typename R>
  constexpr typename return_category_type<R&&>::return_type
  operator()(R&& r) const {
    return (*this)(std::forward<R>(r), return_category_type<R&&>{});
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{

/**
@brief a \ref ranges::view "view" that includes all elements of a \ref ranges::range "range"

A [RangeAdaptorObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorObject) (also a
[RangeAdaptorClosureObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorClosureObject)) that returns a
\ref ranges::view "view" that includes all elements of its \ref ranges::range "range" argument.
 */
VCCC_INLINE_OR_STATIC constexpr detail::all_niebloid all{};

/// @}

} // inline namespace niebloid

namespace detail {

template<typename R, bool = viewable_range<R>::value /* true */>
struct all_t_impl {
  using type = decltype(views::all(std::declval<R>()));
};

template<typename R>
struct all_t_impl<R, false> {};

} // namespace detail


/// @addtogroup ranges
/// @{

/// @brief Calculates the suitable \ref ranges::view "view" type of a `viewable_range` type.
template<typename R>
using all_t = typename detail::all_t_impl<R>::type;

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ALL_HPP
