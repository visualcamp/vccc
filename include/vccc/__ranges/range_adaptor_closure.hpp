//
// Created by yonggyulee on 2023/01/01.
//

#ifndef VCCC_RANGES_RANGE_ADAPTOR_CLOSURE_HPP
#define VCCC_RANGES_RANGE_ADAPTOR_CLOSURE_HPP

#include <cstddef>
#include <type_traits>

#include "vccc/__concepts/derived_from_single_crtp.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__utility/compressed_pair.hpp"

namespace vccc {
namespace ranges {

template<typename Derived>
struct range_adaptor_closure;

namespace detail {

struct basic_range {
  constexpr basic_range() noexcept : data_(0) {}

  constexpr unsigned char* begin() noexcept { return &data_; }
  constexpr const unsigned char* begin() const noexcept { return &data_; }
  constexpr unsigned char* end() noexcept { return &data_ + 1; }
  constexpr const unsigned char* end() const noexcept { return &data_ + 1; }
  constexpr std::size_t size() const noexcept { return 1; }

  unsigned char data_;
};

template<typename C, typename D>
class range_adaptor_closure_object
    : protected compressed_pair<C, D>
    , public range_adaptor_closure<range_adaptor_closure_object<C, D>>
{
  using pair_base = compressed_pair<C, D>;

 public:
  constexpr range_adaptor_closure_object() = default;

  template<typename T, typename U, std::enable_if_t<conjunction<
      std::is_constructible<C, T>, std::is_constructible<D, U>
  >::value, int> = 0>
  constexpr range_adaptor_closure_object(T&& t, U&& u)
      noexcept(conjunction<std::is_nothrow_constructible<C, T>, std::is_nothrow_constructible<D, U>>::value)
      : pair_base(std::forward<T>(t), std::forward<U>(u)) {}

  template<typename... Args, std::enable_if_t<is_invocable<C, Args&&...>::value, int> = 0>
  constexpr invoke_result_t<D, invoke_result_t<C, Args&&...>>
  operator()(Args&&... args) const
      noexcept(conjunction<is_nothrow_invocable<C, Args&&...>,
                           is_nothrow_invocable<D, invoke_result_t<C, Args&&...>> >::value)
  {
    return vccc::invoke(pair_base::second(), vccc::invoke(pair_base::first(), std::forward<Args>(args)...));
  }

};

template<typename C, typename D>
using range_adaptor_closure_object_t = range_adaptor_closure_object<std::remove_reference_t<C>, std::remove_reference_t<D>>;

} // namespace detail

/// @addtogroup ranges
/// @{

/// @brief check if type satisfies range adaptor closure object
template<typename T>
struct is_range_adaptor_closure
    : conjunction<
          derived_from_single_crtp<T, ranges::range_adaptor_closure>,
          is_invocable<T, detail::basic_range>,
          negation< ranges::range<T> >
      > {};

/**
@brief helper base class template for defining a range adaptor closure object

`%vccc::ranges::range_adaptor_closure` is a helper class template for defining a
[RangeAdaptorClosureObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorClosureObject).

@sa [std::ranges::range](https://en.cppreference.com/w/cpp/ranges/range)
*/

template<typename Derived>
struct range_adaptor_closure {
  static_assert(std::is_object<Derived>::value, "Constraints not satisfied");
  static_assert(same_as<Derived, std::remove_cv_t<Derived>>::value, "Constraints not satisfied");

  template<typename Range, typename This, std::enable_if_t<conjunction<
      same_as<Derived, remove_cvref_t<This>>,
      ranges::range<remove_cvref_t<Range>>
  >::value, int> = 0>
  friend constexpr invoke_result_t<This, Range>
  operator|(Range&& r, This&& c)
      noexcept(is_nothrow_invocable<This, Range>::value)
  {
    return vccc::invoke(std::forward<This>(c), std::forward<Range>(r));
  }

  template<typename This, typename Closure, std::enable_if_t<conjunction<
      same_as<Derived, remove_cvref_t<This>>,
      is_range_adaptor_closure<remove_cvref_t<This>>,
      is_range_adaptor_closure<remove_cvref_t<Closure>>,
      std::is_constructible<detail::range_adaptor_closure_object_t<This, Closure>, This, Closure>
  >::value, int> = 0>
  friend constexpr detail::range_adaptor_closure_object_t<This, Closure>
  operator|(This&& c1, Closure&& c2)
      noexcept(std::is_nothrow_constructible<detail::range_adaptor_closure_object_t<This, Closure>, This, Closure>::value)
  {
    return detail::range_adaptor_closure_object_t<This, Closure>(std::forward<This>(c1), std::forward<Closure>(c2));
  }
};


/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_RANGE_ADAPTOR_CLOSURE_HPP
