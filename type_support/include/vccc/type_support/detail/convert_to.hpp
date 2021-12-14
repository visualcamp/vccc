# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
#
# include <type_traits>
# include <utility>
# 
# include "vccc/utility.hpp"
# include "vccc/type_support/at.hpp"
# include "vccc/type_support/detail/traits.h"
# include "vccc/type_traits.hpp"

namespace vccc {
namespace detail {

// Fill rest with zero-initialization
template<typename R, typename T, std::size_t... I, typename IT, IT... I2>
inline R
fill_rest(T&& from, std::index_sequence<I...>, std::integer_sequence<IT, I2...>)
{
  return R(vccc::at<I, std::tuple_element_t<I, R>>(std::forward<T>(from))..., std::tuple_element_t<sizeof...(I) + I2, R>{}...);
}

// default-convertible (std::is_convertible<T, R>::value == true)
template<typename R, typename Dummy, typename T, std::size_t... I>
constexpr inline R
convert_to_impl(std::true_type, Dummy, T&& from, std::index_sequence<I...>) {
  return std::forward<T>(from);
}

// convert to tuple-like to tuple-like
template<typename R, typename T, std::size_t... I>
constexpr inline R
convert_to_impl(std::false_type, std::true_type, T&& from, std::index_sequence<I...> cvt) {
  using un_r = std::decay_t<R>;
  using un_t = std::decay_t<T>;
  using remainders = std::make_index_sequence<(std::tuple_size<un_r>::value > std::tuple_size<un_t>::value ? std::tuple_size<un_r>::value - std::tuple_size<un_t>::value : 0)>;
  return fill_rest<R>(std::forward<T>(from), cvt, remainders{});
}

// convert (tuple-like || range) to (range || tuple-like)
template<typename R, typename T, std::size_t... I>
constexpr inline R
convert_to_impl(std::false_type, std::false_type, T&& from, std::index_sequence<I...>) {
  using type = typename R::value_type;
  return {vccc::at<I, type>(from)...};
}

} // namespace detail
} // namespace vccc

# endif //VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP