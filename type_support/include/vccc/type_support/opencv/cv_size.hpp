# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_support/opencv/detail/cv_size.hpp"
# include "vccc/type_traits.hpp"

namespace vc{


/** cv_size */

template<typename T>
struct detail::cv_size<cv::Size_<T>> : cv_size_n<2> {};


/** at */

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
T&
at(cv::Size_<T>& size) {
  return size.width;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
const T&
at(const cv::Size_<T>& size) {
  return size.width;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
T&&
at(cv::Size_<T>&& size) {
  return std::move(size.width);
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
const T&&
at(const cv::Size_<T>&& size) {
  return std::move(size.width);
}

// index = 1
template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
T&
at(cv::Size_<T>& size) {
  return size.height;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
const T&
at(const cv::Size_<T>& size) {
  return size.height;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
T&&
at(cv::Size_<T>&& size) {
  return std::move(size.height);
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
const T&&
at(const cv::Size_<T>&& size) {
  return std::move(size.height);
}

}

#endif //VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
