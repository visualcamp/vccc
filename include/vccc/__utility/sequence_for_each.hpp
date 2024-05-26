//
// Created by YongGyu Lee on 2024. 5. 17.
//

#ifndef VCCC_UTILITY_SEQUENCE_FOR_EACH_HPP_
#define VCCC_UTILITY_SEQUENCE_FOR_EACH_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace detail {

template<typename Seq, typename F>
struct sequence_for_each_invocable : std::false_type {};

template<typename T, T... v, typename F>
struct sequence_for_each_invocable<std::integer_sequence<T, v...>, F>
    : conjunction<
          is_invocable<F, std::integral_constant<T, v>>...
      > {};

} // namespace detail

/// @addtogroup utility
/// @{

/// @brief performs f(std::integral_constant<T, i>{}) for i in v...
template<typename T, T... v, typename F>
constexpr std::enable_if_t<detail::sequence_for_each_invocable<std::integer_sequence<T, v...>, F>::value>
sequence_for_each(std::integer_sequence<T, v...>, F&& f) {
  int dummy[] = {
      (vccc::invoke(f, std::integral_constant<T, v>{}), 0)...
  };
  (void)dummy;
}

/// @brief performs f(std::integral_constant<T, i>{}) for i in [0, N)
template<typename T, T N, typename F>
constexpr std::enable_if_t<detail::sequence_for_each_invocable<std::make_integer_sequence<T, N>, F>::value>
sequence_for_each(F&& f) {
  return vccc::sequence_for_each(std::make_integer_sequence<T, N>{}, std::forward<F>(f));
}

/// @brief performs f(std::integral_constant<std::size_t, i>{}) for i in [0, N)
template<std::size_t N, typename F>
constexpr std::enable_if_t<detail::sequence_for_each_invocable<std::make_index_sequence<N>, F>::value>
sequence_for_each(F&& f) {
  return vccc::sequence_for_each(std::make_index_sequence<N>{}, std::forward<F>(f));
}

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_SEQUENCE_FOR_EACH_HPP_
