# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP
#
# include <utility>
# include "vccc/utility.hpp"
# include "vccc/type_support/at.hpp"

namespace vccc{namespace detail{

// fill rest with integer_sequence
template<typename R, typename T, std::size_t... I, typename IT, IT... I2>
R
fill_rest(const T& from, std::index_sequence<I...> index_seq, std::integer_sequence<IT, I2...> rest_seq)
{
  return R(vccc::at<I, typename R::value_type>(from)..., I2...);
}

// convert to cv type
template<typename R, typename T, std::size_t... I>
R
convert_to_impl(std::true_type, const T& from, std::index_sequence<I...> index_seq)
{
  using Zeros = make_zero_sequence<((cv_size_v<R> > cv_size_v<T> && is_cv_type_v<T>) ? diff_cv_size_v<R, T> : 0)>;
  return fill_rest<R>(from, index_seq, Zeros{});
}

// convert to non-cv type (such as container) with initializer-list
template<typename R, typename T, std::size_t... I>
R
convert_to_impl(std::false_type, const T& from, std::index_sequence<I...>)
{
  return {vccc::at<I, vccc::vtype_t<R>>(from)...};
}

}}

# endif //VCCC_TYPE_SUPPORT_DETAIL_CONVERT_TO_HPP