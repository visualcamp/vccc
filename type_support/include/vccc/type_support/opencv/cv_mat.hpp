# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_support/opencv/detail/cv_size.hpp"

namespace vc{

//template<typename T, int m, int n> struct detail::cv_size<cv::Matx<T, m, n>>;
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T& at(cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T& at(const cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T&& at(cv::Matx<T, m, n>&& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T&& at(const cv::Matx<T, m, n>&& matx);

/** cv_size */

template<typename T, int m, int n>
struct detail::cv_size<cv::Matx<T, m, n>> : cv_size_n<m*n> {};


/** at */

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vc::at<> (cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vc::at<> (const cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vc::at<> (cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx&&)");
  return std::move(matx(i, j));
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
const T&&
at(const cv::Matx<T, m, n>&& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vc::at<> (const cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx&&)");
  return std::move(matx(i, j));
}


template<std::size_t i,
    typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx)");
  return matx(i);
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (const cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx)");
  return matx(i);
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx&&)");
  return std::move(matx(i));
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
const T&&
at(const cv::Matx<T, m, n>&& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (const cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx&&)");
  return std::move(matx(i));
}


/** add */

template<typename T, int m, int n, typename N>
cv::Matx<T, m, n> add(const cv::Matx<T, m, n>& matx, N n_){
  return matx + cv::Matx<T, m, n>::all(n_);
}

}

#endif //VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
