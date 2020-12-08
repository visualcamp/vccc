//
// Created by YongGyu Lee on 2020/12/08.
//

#ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP
#define VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP

#include "opencv2/core/types.hpp"
#include "vccc/type_traits.hpp"

namespace vc{

/** cv_size */

template<typename T>
struct detail::cv_size<cv::Point3_<T>> : cv_size_n<3> {};


/** at */

// index = 0
template<std::size_t i, typename T,
    VCCC_REQUIRE(i==0)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.x;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i==0)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.x;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i==0)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.x);
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 0)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.x);
}

// index = 1
template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 1)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.y;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 1)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.y;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 1)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.y);
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 1)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.y);
}

// index = 2
template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 2)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.z;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 2)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.z;
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 2)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.z);
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i == 2)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.z);
}


/** add */

template<typename T, typename N>
cv::Point3_<T> add(const cv::Point3_<T>& point3, N n) {
  return cv::Point3_<T>(point3.x + n, point3.y + n, point3.z + n);
}

}

#endif //VCCC_TYPE_SUPPORT_OPENCV_CV_POINT3_HPP
