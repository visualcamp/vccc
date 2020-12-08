# /*
# Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_support/opencv/detail/cv_size.hpp"

namespace vc{

/** cv_size */

template<typename T, int cn>
struct detail::cv_size<cv::Vec<T, cn>> : cv_size_n<cn> {};


/** at */

template<std::size_t i, typename T, int n>
constexpr
T&
at(cv::Vec<T, n>& vec) {
  static_assert(i < n, "Index out of bounds in vc::at<> (cv::Vec)");
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr
const T&
at(const cv::Vec<T, n>& vec) {
  static_assert(i < n, "Index out of bounds in vc::at<> (const cv::Vec)");
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr
T&&
at(cv::Vec<T, n>&& vec) {
  static_assert(i < n, "Index out of bounds in vc::at<> (cv::Vec&&)");
  return std::move(vec[i]);
}

template<std::size_t i, typename T, int n>
constexpr
const T&&
at(const cv::Vec<T, n>&& vec) {
  static_assert(i < n, "Index out of bounds in vc::at<> (const cv::Vec&&)");
  return std::move(vec[i]);
}


/** add */

template<typename T, int cn, typename N>
cv::Vec<T, cn> add(const cv::Vec<T, cn>& vec, N n) {
  return vec + cv::Vec<T, cn>::all(n);
}

}

#endif //VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
