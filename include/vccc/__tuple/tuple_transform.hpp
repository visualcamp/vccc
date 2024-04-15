//
// Created by YongGyu Lee on 2/20/24.
//

#ifndef VCCC_TUPLE_TUPLE_TRANSFORM_HPP
#define VCCC_TUPLE_TUPLE_TRANSFORM_HPP

#include <tuple>
#include <utility>

#include "vccc/__functional/invoke.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename Tuple, typename F, std::size_t... I>
constexpr auto tuple_transform_impl(Tuple&& t, F&& f, std::index_sequence<I...>)
    noexcept(conjunction<
        bool_constant<noexcept(vccc::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))))>...
    >::value)
{
  return std::tuple<decltype(vccc::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))))...>(
      vccc::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t)))...);
}

} // namespace detail

/// @addtogroup tuple
/// @{

/// @brief Constructs a new tuple with each elements transformed
template<typename Tuple, typename F>
constexpr auto tuple_transform(Tuple&& t, F&& f)
    noexcept(noexcept(
        detail::tuple_transform_impl(
            std::forward<Tuple>(t),
            std::forward<F>(f),
            std::make_index_sequence<std::tuple_size<remove_cvref_t<Tuple>>::value>{}
        )
    ))
{
  return detail::tuple_transform_impl(
      std::forward<Tuple>(t), std::forward<F>(f),
      std::make_index_sequence<std::tuple_size<remove_cvref_t<Tuple>>::value>{});
}

/// @}

} // namespace vccc

#endif // VCCC_TUPLE_TUPLE_TRANSFORM_HPP
