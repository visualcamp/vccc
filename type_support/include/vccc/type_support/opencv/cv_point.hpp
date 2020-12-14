# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
#
# include <type_traits>
# include "opencv2/core/types.hpp"

namespace vccc{
//
//template<typename T> struct detail::cv_size<cv::Point_<T>>;
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 0)> constexpr       T&  at(      cv::Point_<T>&  point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 0)> constexpr const T&  at(const cv::Point_<T>&  point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 0)> constexpr       T&& at(      cv::Point_<T>&& point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 0)> constexpr const T&& at(const cv::Point_<T>&& point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 1)> constexpr       T&  at(      cv::Point_<T>&  point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 1)> constexpr const T&  at(const cv::Point_<T>&  point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 1)> constexpr       T&& at(      cv::Point_<T>&& point2);
//template<std::size_t i, typename T, VCCC_REQUIRE_FORWARD(i == 1)> constexpr const T&& at(const cv::Point_<T>&& point2);
//


/** cv_size */

template<typename T>
struct detail::cv_size<cv::Point_<T>> : cv_size_n<2> {};


/** at */

// index = 0
template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
T&
at(cv::Point_<T>& point2) {
  return point2.x;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
const T&
at(const cv::Point_<T>& point2) {
  return point2.x;
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
T&&
at(cv::Point_<T>&& point2) {
  return std::move(point2.x);
}

template<std::size_t i, typename T, std::enable_if_t<i==0, int> = 0>
constexpr
const T&&
at(const cv::Point_<T>&& point2) {
  return std::move(point2.x);
}

// index = 1
template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
T&
at(cv::Point_<T>& point2) {
  return point2.y;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
const T&
at(const cv::Point_<T>& point2) {
  return point2.y;
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
T&&
at(cv::Point_<T>&& point2) {
  return std::move(point2.y);
}

template<std::size_t i, typename T, std::enable_if_t<i==1, int> = 0>
constexpr
const T&&
at(const cv::Point_<T>&& point2) {
  return std::move(point2.y);
}


/** add */

template<typename T, typename N>
cv::Point_<T> add(const cv::Point_<T>& point2, N n) {
  return cv::Point_<T>(point2.x + n, point2.y + n);
}


/** resize */

template<int new_size, typename T, std::enable_if_t<new_size == 2, int> = 0>
auto resize(const cv::Point_<T>& from){
  return from;
}

template<int new_size, typename T, std::enable_if_t<new_size==3, int> = 0>
auto resize(const cv::Point_<T>& from){
  return cv::Point3_<T>(from.x, from.y, 0);
}

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
