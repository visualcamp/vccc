# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_RESIZE_HPP
# define VCCC_TYPE_RESIZE_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_traits.hpp"
# include "vccc/type_support/convert_to.hpp"

namespace vccc{


template<int new_size, typename T, int old_size>
auto resize(const cv::Vec<T, old_size>& from){
  return convert_to<cv::Vec<T, new_size>>(from);
}


/** any resizable type */

template<int new_size, typename T, typename = void_t<decltype(std::declval<T>().resize(new_size))>>
decltype(auto) resize(T&& t){
  auto copy_or_same = std::forward<T>(t);
  copy_or_same.resize(new_size);
  return copy_or_same;
}

}

# endif //VCCC_TYPE_RESIZE_HPP
