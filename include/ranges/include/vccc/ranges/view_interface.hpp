//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_VIEW_INTERFACE_HPP_
#define VCCC_RANGES_VIEW_INTERFACE_HPP_

#include <type_traits>

#include "vccc/memory/to_address.hpp"
#include "vccc/iterator/contiguous_iterator.hpp"
#include "vccc/ranges/empty.hpp"
#include "vccc/ranges/forward_range.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
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

} // namespace detail

template<typename Derived>
class view_interface {
 public:
  static_assert(std::is_class<Derived>::value && std::is_same<Derived, std::remove_cv_t<Derived>>::value,
                "Constraints not satisfied");

  /// empty
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::sized_range<Derived>
  >::value, int> = 0>
  constexpr bool empty() {
    return ranges::size(derived()) == 0;
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<ranges::sized_range<Derived>>,
      ranges::forward_range<Derived>
  >::value, int> = 0>
  constexpr bool empty() {
    return ranges::begin(derived()) == ranges::end(derived());
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      ranges::sized_range<const Derived>
  >::value, int> = 0>
  constexpr bool empty() const {
    return ranges::size(derived()) == 0;
  }
  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<ranges::sized_range<const Derived>>,
      ranges::forward_range<const Derived>
  >::value, int> = 0>
  constexpr bool empty() const {
    return ranges::begin(derived()) == ranges::end(derived());
  }


  /// operator bool()
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

  /// data()
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

 private:

  const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }

  Derived& derived() {
    return static_cast<Derived&>(*this);
  }
};

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEW_INTERFACE_HPP_
