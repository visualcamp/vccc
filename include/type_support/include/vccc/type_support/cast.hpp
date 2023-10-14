# /*
#  * Created by YongGyu Lee on 2021/03/17.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_CAST_HPP
# define VCCC_TYPE_SUPPORT_CAST_HPP
#
# include "vccc/type_support/library_config.hpp"
# include "vccc/type_traits.hpp"
# include <cstddef>

namespace vccc {

namespace internal {
namespace type_support {

template<typename To, typename From, typename = void>
struct saturate_cast_possible :
# if VCCC_USE_OPENCV_FEATURES
  public std::integral_constant<bool, are_arithmetic<std::decay_t<To>, std::decay_t<From>>::value>
# else
  public std::false_type
# endif
        {};

template<typename To, typename From>
using saturate_cast_possible_t = typename saturate_cast_possible<To, From>::type;

template<typename To, typename From>
constexpr inline
decltype(auto)
cast(std::false_type, From&& from) {
  return static_cast<To>(std::forward<From>(from));
}

template<typename To, typename From>
constexpr inline
decltype(auto)
cast(std::true_type, From from) {
# if VCCC_USE_OPENCV_FEATURES
  return cv::saturate_cast<To>(from);
# else
  cast(std::false_type{}, from);
# endif
}


} // namespace type_support
} // namespace internal

template<typename ToType, typename FromType>
constexpr inline
decltype(auto)
cast(FromType&& from) {
  return internal::type_support::cast<ToType>(
      internal::type_support::saturate_cast_possible_t<ToType, FromType>{},
      std::forward<FromType>(from));
}

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_CAST_HPP
