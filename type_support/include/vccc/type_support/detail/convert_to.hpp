# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
#
# include <tuple>
# include <utility>
# 
# include "vccc/utility.hpp"
# include "vccc/type_traits.hpp"
# include "vccc/type_support/at.hpp"

namespace vccc{
namespace detail{

template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, void_t<decltype(std::declval<T&>().size())>> : std::true_type {};

template<size_t sz, typename T> inline bool check_bound(std::true_type, const T& from) { return from.size() >= sz; }
template<size_t sz, typename T> inline bool check_bound(std::false_type, const T&) { return true; }

template<typename T, bool v = is_tuple_like<T>::value>
struct tuple_size_or_zero : std::integral_constant<size_t, 0> {};

template<typename T>
struct tuple_size_or_zero<T, true> : std::tuple_size<T> {};


// fill rest with integer_sequence
template<typename R, typename T, std::size_t... I, typename IT, IT... I2>
inline R
fill_rest(const T& from, std::index_sequence<I...> index_seq, std::integer_sequence<IT, I2...> rest_seq)
{
  return R(vccc::at<I, typename R::value_type>(from)..., I2...);
}

// convert to cv type
template<typename R, typename T, std::size_t... I>
inline R
convert_to_impl(std::true_type, const T& from, std::index_sequence<I...> index_seq)
{
  ENSURES(check_bound<std::tuple_size<R>::value>(bool_constant<!is_tuple_like<T>::value && has_size<T>::value>{}, from),
          "Following must be satisfied: from.size() >= tuple_size<R>::value");
  using Zeros = make_zero_sequence<(
    (tuple_size_or_zero<R>::value > tuple_size_or_zero<T>::value && is_tuple_like<T>::value)
        ? static_diff<size_t, tuple_size_or_zero<R>::value, tuple_size_or_zero<T>::value>::value : 0)>;
  return fill_rest<R>(from, index_seq, Zeros{});
}

// convert to non-cv type (such as container) with initializer-list
template<typename R, typename T, std::size_t... I>
inline R
convert_to_impl(std::false_type, const T& from, std::index_sequence<I...>)
{
  return {vccc::at<I, vccc::vtype_t<R>>(from)...};
}

}}

# endif //VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP