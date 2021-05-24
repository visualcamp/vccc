# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP
#
# include "opencv2/core/types.hpp"
# include "vccc/type_traits.hpp"

namespace vccc{


/**
@addtogroup type_support_cv_size
@{

@defgroup type_support_cv_size_cv_point3 cv_size (cv::Point3_)
@addtogroup type_support_cv_size_cv_point3
@{
*/
template<typename T>
struct cv_size<cv::Point3_<T>> : cv_size_n<3> {};
//! @} type_support_cv_size_cv_point3
//! @} type_support_cv_size


/**
@addtogroup type_support_at
@{
    @defgroup type_support_at_cv_point3 vccc::at (cv::Point3_)
    Index-based value accessor
@}

@addtogroup type_support_at_cv_point3
@{
*/

// index = 0
template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
T&
at(cv::Point3_<T>& point3)
{
  return point3.x;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
const T&
at(const cv::Point3_<T>& point3)
{
  return point3.x;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
T&&
at(cv::Point3_<T>&& point3)
{
  return std::move(point3.x);
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr inline
const T&&
at(const cv::Point3_<T>&& point3)
{
  return std::move(point3.x);
}

// index = 1
template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
T&
at(cv::Point3_<T>& point3)
{
  return point3.y;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
const T&
at(const cv::Point3_<T>& point3)
{
  return point3.y;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
T&&
at(cv::Point3_<T>&& point3)
{
  return std::move(point3.y);
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr inline
const T&&
at(const cv::Point3_<T>&& point3)
{
  return std::move(point3.y);
}

// index = 2
template<std::size_t i, typename T, std::enable_if_t<i==2, int> = 0>
constexpr inline
T&
at(cv::Point3_<T>& point3)
{
  return point3.z;
}

template<std::size_t i, typename T, std::enable_if_t<i==2, int> = 0>
constexpr inline
const T&
at(const cv::Point3_<T>& point3)
{
  return point3.z;
}

template<std::size_t i, typename T, std::enable_if_t<i==2, int> = 0>
constexpr inline
T&&
at(cv::Point3_<T>&& point3)
{
  return std::move(point3.z);
}

template<std::size_t i, typename T, std::enable_if_t<i==2, int> = 0>
constexpr inline
const T&&
at(const cv::Point3_<T>&& point3)
{
  return std::move(point3.z);
}

//! @} type_support_at_cv_point3

/** add */

template<typename T, typename N>
inline cv::Point3_<T> add(const cv::Point3_<T>& point3, N n)
{
  return cv::Point3_<T>(point3.x + n, point3.y + n, point3.z + n);
}


/** resize */

template<int new_size, typename T, VCCC_ENABLE_IF(new_size==2)>
inline auto resize(const cv::Point3_<T>& from){
  return cv::Point_<T>(from.x, from.y);
}

template<int new_size, typename T, VCCC_ENABLE_IF(new_size==3)>
inline auto resize(const cv::Point3_<T>& from){
  return from;
}

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP
