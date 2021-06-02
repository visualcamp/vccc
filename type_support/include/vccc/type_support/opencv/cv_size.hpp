# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
#
# include "opencv2/opencv.hpp"
# include "vccc/type_support/opencv/traits.hpp"
# include "vccc/type_traits.hpp"

namespace vccc{


/**
@addtogroup type_support_cv_size
@{

@defgroup type_support_cv_size_cv_size cv_size (cv::Size)
@addtogroup type_support_cv_size_cv_size
@{
*/
template<typename T>
struct cv_size<cv::Size_<T>> : cv_size_n<2> {};
//! @} type_support_cv_size_cv_size
//! @} type_support_cv_size


/**
@addtogroup type_support_at
@{
    @defgroup type_support_at_cv_size vccc::at (cv::Size)
    Index-based value accessor
@}

@addtogroup type_support_at_cv_size
@{
*/

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
T&
at(cv::Size_<T>& size)
{
  return size.width;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
const T&
at(const cv::Size_<T>& size)
{
  return size.width;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
T&&
at(cv::Size_<T>&& size)
{
  return std::move(size.width);
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
const T&&
at(const cv::Size_<T>&& size)
{
  return std::move(size.width);
}

// index = 1
template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
T&
at(cv::Size_<T>& size)
{
  return size.height;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
const T&
at(const cv::Size_<T>& size)
{
  return size.height;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
T&&
at(cv::Size_<T>&& size)
{
  return std::move(size.height);
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
const T&&
at(const cv::Size_<T>&& size)
{
  return std::move(size.height);
}

//! @} type_support_at_cv_size

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
