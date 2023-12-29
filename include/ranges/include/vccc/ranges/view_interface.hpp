//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_VIEW_INTERFACE_HPP_
#define VCCC_RANGES_VIEW_INTERFACE_HPP_

#include <type_traits>

#include "vccc/memory/to_address.hpp"
#include "vccc/iterator/contiguous_iterator.hpp"
#include "vccc/iterator/prev.hpp"
#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/bidirectional_range.hpp"
#include "vccc/ranges/common_range.hpp"
#include "vccc/ranges/empty.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/forward_range.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/random_access_range.hpp"
#include "vccc/ranges/range_difference_t.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/ranges/sentinel_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, typename = void>
struct is_empty_callable : std::false_type {};

template<typename T>
struct is_empty_callable<T, void_t<decltype( ranges::empty(std::declval<T>()) )>> : std::true_type {};

template<typename T, bool = has_typename_type<ranges::iterator<T>>::value>
struct check_vi_data : std::false_type {};
template<typename T>
struct check_vi_data<T, true> : contiguous_iterator<ranges::iterator_t<T>> {};

template<
    typename T,
    bool =
      conjunction<
        ranges::forward_range<T>,
        has_typename_type<ranges::iterator<T>>,
        has_typename_type<ranges::sentinel<T>>
    >::value
>
struct check_vi_size : std::false_type {};

template<typename T>
struct check_vi_size<T, true> : sized_sentinel_for<ranges::sentinel_t<T>, ranges::iterator_t<T>> {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief helper class template for defining a `view`, using the
[curiously recurring template pattern](https://en.cppreference.com/w/cpp/language/crtp)

`%vccc::ranges::view_interface` is a helper class template for defining a view interface.

`%view_interface` is typically used with [CRTP](https://en.cppreference.com/w/cpp/language/crtp):

@sa [std::ranges::view_interface](https://en.cppreference.com/w/cpp/ranges/view_interface)
 */
template<typename Derived>
class view_interface {
 public:
  static_assert(std::is_class<Derived>::value && std::is_same<Derived, std::remove_cv_t<Derived>>::value,
                "Constraints not satisfied");


  /**
   * @name empty
   * @brief returns whether the derived view is empty. Provided if it satisfies `sized_range` or `forward_range`.
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::sized_range<Derived>
  >::value, int> = 0>
  constexpr bool empty() {
    using namespace vccc::rel_ops;
    return ranges::size(derived()) == 0;
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<ranges::sized_range<Derived>>,
      ranges::forward_range<Derived>
  >::value, int> = 0>
  constexpr bool empty() {
    using namespace vccc::rel_ops;
    return ranges::begin(derived()) == ranges::end(derived());
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::sized_range<const Derived>
  >::value, int> = 0>
  constexpr bool empty() const {
    using namespace vccc::rel_ops;
    return ranges::size(derived()) == 0;
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<ranges::sized_range<const Derived>>,
      ranges::forward_range<const Derived>
  >::value, int> = 0>
  constexpr bool empty() const {
    using namespace vccc::rel_ops;
    return ranges::begin(derived()) == ranges::end(derived());
  }
  /// @}



  /**
   * @name operator bool
   * @brief returns whether the derived view is not empty. Provided if `ranges::empty` is applicable to it.
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      detail::is_empty_callable<Derived&>
  >::value, int> = 0>
  constexpr explicit operator bool() {
    return !ranges::empty(derived());
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
    detail::is_empty_callable<const Derived&>
  >::value, int> = 0>
  constexpr explicit operator bool() const {
    return !ranges::empty(derived());
  }
  /// @}


  /**
   * @name data
   * @brief gets the address of derived view's data. Provided if its iterator type satisfies `contiguous_iterator`.
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      detail::check_vi_data<Derived>
  >::value, int> = 0>
  constexpr auto data() {
    return vccc::to_address(ranges::begin(derived()));
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      detail::check_vi_data<const Derived>
  >::value, int> = 0>
  constexpr auto data() const {
    return vccc::to_address(ranges::begin(derived()));
  }
  /// @}


  /**
   * @name size
   * \brief returns the number of elements in the derived view. Provided if it satisfies `forward_range` and its
   * sentinel and iterator type satisfy `sized_sentinel_for`.
   *
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      detail::check_vi_size<Derived>
  >::value, int> = 0>
  constexpr auto size() {
    using T = decltype(ranges::end(derived()) - ranges::begin(derived()));
    return static_cast<T>(ranges::end(derived()) - ranges::begin(derived()));
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      detail::check_vi_size<const Derived>
  >::value, int> = 0>
  constexpr auto size() const {
    using T = decltype(ranges::end(derived()) - ranges::begin(derived()));
    return static_cast<T>(ranges::end(derived()) - ranges::begin(derived()));
  }
  /// @}


  /**
   * @name front
   * \brief returns the number of elements in the derived view. Provided if it satisfies `forward_range` and its
   * sentinel and iterator type satisfy `sized_sentinel_for`.
   *
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::forward_range<Derived>
  >::value, int> = 0>
  constexpr decltype(auto) front() {
    return *ranges::begin(derived());
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::forward_range<const Derived>
  >::value, int> = 0>
  constexpr decltype(auto) front() const {
    return *ranges::begin(derived());
  }
  /// @}


  /**
   * @name back
   * \brief returns the last element in the derived view.
   * Provided if it satisfies `bidirectional_range` and `common_range`.
   *
   * The default implementation of `%back()` member function returns the last element in the view of the derived type.
   * Whether the element is returned by value or by reference depends on the `operator*` of the iterator type.
   * @{
   */
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::bidirectional_range<Derived>,
      ranges::common_range<Derived>
  >::value, int> = 0>
  constexpr decltype(auto) back() {
    return *ranges::prev(ranges::end(derived()));
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::bidirectional_range<const Derived>,
      ranges::common_range<const Derived>
  >::value, int> = 0>
  constexpr decltype(auto) back() const {
    return *ranges::prev(ranges::end(derived()));
  }
  /// @}


  /**
   * @name operator[]
   * \brief returns the \f$ { N^{th}} \f$ element in the derived view. Provided if it satisfies `random_access_range`.
   *
   * The default implementation of `%operator[]` member function obtains the element at the specified offset relative
   * to the beginning iterator, reusing the `%operator[]` of the iterator type.
   * @{
   */
  template<typename R = Derived, typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::random_access_range<R>
  >::value, int> = 0>
  constexpr decltype(auto) operator[](ranges::range_difference_t<R> n) {
    ranges::begin(derived())[n];
  }
  template<typename R = const Derived, typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::random_access_range<R>
  >::value, int> = 0>
  constexpr decltype(auto) operator[](ranges::range_difference_t<R> n) const {
    ranges::begin(derived())[n];
  }
  /// @}

 private:
  constexpr const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }

  constexpr Derived& derived() {
    return static_cast<Derived&>(*this);
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEW_INTERFACE_HPP_
