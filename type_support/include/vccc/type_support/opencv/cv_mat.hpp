# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
#
# include "opencv2/opencv.hpp"
# include "vccc/type_support/opencv/traits.hpp"

namespace vccc{

//template<typename T, int m, int n> struct detail::cv_size<cv::Matx<T, m, n>>;
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T& at(cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T& at(const cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T&& at(cv::Matx<T, m, n>&& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T&& at(const cv::Matx<T, m, n>&& matx);

/**
@addtogroup type_support_cv_size
@{

@defgroup type_support_cv_size_cv_matx cv_size (cv::Matx)
@addtogroup type_support_cv_size_cv_matx
@{
*/
template<typename T, int m, int n>
struct cv_size<cv::Matx<T, m, n>> : cv_size_n<m*n> {};
//! @} type_support_cv_size_cv_matx
//! @} type_support_cv_size

/**
@addtogroup type_support_at
@{
    @defgroup type_support_at_cv_matx vccc::at (cv::Matx)
    Index-based value accessor
@}

@addtogroup type_support_at_cv_matx
@{
*/

template<std::size_t i, std::size_t j,
         typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx)
{
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx)
{
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (const cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx)
{
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx&&)");
  return std::move(matx(i, j));
}

template<std::size_t i, std::size_t j,
    typename T, int m, int n>
constexpr
const T&&
at(const cv::Matx<T, m, n>&& matx)
{
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (const cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx&&)");
  return std::move(matx(i, j));
}


template<std::size_t i,
    typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx)
{
  static_assert((i < m * n), "Index out of bounds in vccc::at<> (cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx)");
  return matx(i);
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx)
{
  static_assert((i < m * n), "Index out of bounds in vccc::at<> (const cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx)");
  return matx(i);
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx)
{
  static_assert((i < m * n), "Index out of bounds in vccc::at<> (cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx&&)");
  return std::move(matx(i));
}

template<std::size_t i,
    typename T, int m, int n>
constexpr
const T&&
at(const cv::Matx<T, m, n>&& matx)
{
  static_assert((i < m * n), "Index out of bounds in vccc::at<> (const cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx&&)");
  return std::move(matx(i));
}

//! @} type_support_at_cv_matx

/** add */

template<typename T, int m, int n, typename N>
cv::Matx<T, m, n> add(const cv::Matx<T, m, n>& matx, N n_)
{
  return matx + cv::Matx<T, m, n>::all(n_);
}


}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP