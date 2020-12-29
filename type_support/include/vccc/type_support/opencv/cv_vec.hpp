# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_support/opencv/detail/cv_size.hpp"

namespace vccc{

/**
@addtogroup type_support_cv_size
@{

@defgroup type_support_cv_size_cv_vec cv_size (cv::Vec)
@addtogroup type_support_cv_size_cv_vec
@{
*/
template<typename T, int cn>
struct detail::cv_size<cv::Vec<T, cn>> : cv_size_n<cn> {};
//! @} type_support_cv_size_cv_vec
//! @} type_support_cv_size


/**
@addtogroup type_support_at
@{
    @defgroup type_support_at_cv_vec vccc::at (cv::Vec)
    Index-based value accessor
@}

@addtogroup type_support_at_cv_vec
@{
*/

template<std::size_t i, typename T, int n>
constexpr
T&
at(cv::Vec<T, n>& vec)
{
  static_assert(i < n, "Index out of bounds in vccc::at<> (cv::Vec)");
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr
const T&
at(const cv::Vec<T, n>& vec)
{
  static_assert(i < n, "Index out of bounds in vccc::at<> (const cv::Vec)");
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr
T&&
at(cv::Vec<T, n>&& vec)
{
  static_assert(i < n, "Index out of bounds in vccc::at<> (cv::Vec&&)");
  return std::move(vec[i]);
}

template<std::size_t i, typename T, int n>
constexpr
const T&&
at(const cv::Vec<T, n>&& vec)
{
  static_assert(i < n, "Index out of bounds in vccc::at<> (const cv::Vec&&)");
  return std::move(vec[i]);
}

//! @} type_support_at_cv_vec

/** add */

template<typename T, int cn, typename N>
cv::Vec<T, cn> add(const cv::Vec<T, cn>& vec, N n)
{
  auto copy = vec;
  for(int i=0; i<cn; ++i)
    copy[i] += n;
  return copy;
}

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
