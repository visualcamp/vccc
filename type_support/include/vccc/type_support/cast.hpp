# /*
#  * Created by YongGyu Lee on 2021/03/17.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_CAST_HPP_
# define VCCC_TYPE_SUPPORT_CAST_HPP_
#
# include "vccc/type_support/library_config.hpp"
# include <cstddef>

namespace vccc {

template<typename ToType, typename FromType>
constexpr inline
decltype(auto)
cast(FromType&& from) {
# if VCCC_USE_OPENCV_FEATURES
  return cv::saturate_cast<ToType>(std::forward<FromType>(from));
# else
  return static_cast<ToType>(std::forward<FromType>(from));
# endif
}

}

# endif //VCCC_TYPE_SUPPORT_CAST_HPP_
