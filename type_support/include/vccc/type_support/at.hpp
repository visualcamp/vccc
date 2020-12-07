//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_SUPPORT_AT_HPP
#define VCCC_TYPE_SUPPORT_AT_HPP

#include "opencv2/core/types.hpp"
#include "vccc/type_traits.hpp"

namespace vc{

/** cv::Point_ */
// index = 0
template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&
at(cv::Point_<T>& point2) {
  return point2.x;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&
at(const cv::Point_<T>& point2) {
  return point2.x;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&&
at(cv::Point_<T>&& point2) {
  return std::move(point2.x);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&&
at(const cv::Point_<T>&& point2) {
  return std::move(point2.x);
}

// index = 1
template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&
at(cv::Point_<T>& point2) {
  return point2.y;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&
at(const cv::Point_<T>& point2) {
  return point2.y;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&&
at(cv::Point_<T>&& point2) {
  return std::move(point2.y);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&&
at(const cv::Point_<T>&& point2) {
  return std::move(point2.y);
}


/** cv::Point3_ */
// index = 0

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.x;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.x;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.x);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.x);
}

// index = 1
template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.y;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.y;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.y);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.y);
}

// index = 2
template<std::size_t i, typename T, VCCC_REQUIRE(i == 2)>
constexpr
T&
at(cv::Point3_<T>& point3) {
  return point3.z;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 2)>
constexpr
const T&
at(const cv::Point3_<T>& point3) {
  return point3.z;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 2)>
constexpr
T&&
at(cv::Point3_<T>&& point3) {
  return std::move(point3.z);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 2)>
constexpr
const T&&
at(const cv::Point3_<T>&& point3) {
  return std::move(point3.z);
}


/** cv::Vec */

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


/** cv::Matx */

template<std::size_t row, std::size_t col, typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx) {
  static_assert(row < m && col < n, "Index out of bounds in vc::at<> (cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx)");
  return matx(row, col);
}

template<std::size_t row, std::size_t col, typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx) {
  static_assert(row < m && col < n, "Index out of bounds in vc::at<> (const cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx)");
  return matx(row, col);
}

template<std::size_t row, std::size_t col, typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx) {
  static_assert(row < m && col < n, "Index out of bounds in vc::at<> (cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx&&)");
  return std::move(matx(row, col));
}

template<std::size_t row, std::size_t col, typename T, int m, int n>
constexpr
const T&&
at(cv::Matx<T, m, n>& matx) {
  static_assert(row < m && col < n, "Index out of bounds in vc::at<> (const cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx&&)");
  return std::move(matx(row, col));
}


template<std::size_t i, typename T, int m, int n>
constexpr
T&
at(cv::Matx<T, m, n>& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx)");
  return matx(i);
}

template<std::size_t i, typename T, int m, int n>
constexpr
const T&
at(const cv::Matx<T, m, n>& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (const cv::Matx)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx)");
  return matx(i);
}

template<std::size_t i, typename T, int m, int n>
constexpr
T&&
at(cv::Matx<T, m, n>&& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (cv::Matx&&)");
  return std::move(matx(i));
}

template<std::size_t i, typename T, int m, int n>
constexpr
const T&&
at(const cv::Matx<T, m, n>&& matx) {
  static_assert((i < m * n), "Index out of bounds in vc::at<> (const cv::Matx&&)");
  static_assert((m == 1 || n == 1), "Invalid size (const cv::Matx&&)");
  return std::move(matx(i));
}


/** cv::Size_ */

// index = 0
template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&
at(cv::Size_<T>& size) {
  return size.width;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&
at(const cv::Size_<T>& size) {
  return size.width;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
T&&
at(cv::Size_<T>&& size) {
  return std::move(size.width);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 0)>
constexpr
const T&&
at(const cv::Size_<T>&& size) {
  return std::move(size.width);
}

// index = 1
template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&
at(cv::Size_<T>& size) {
  return size.height;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&
at(const cv::Size_<T>& size) {
  return size.height;
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
T&&
at(cv::Size_<T>&& size) {
  return std::move(size.height);
}

template<std::size_t i, typename T, VCCC_REQUIRE(i == 1)>
constexpr
const T&&
at(const cv::Size_<T>&& size) {
  return std::move(size.height);
}


/**
 * return cv::saturate_cast<C>(at<i,...>(t))
 */

template<std::size_t i, typename C, typename T>
constexpr
decltype(auto)
at(const T& t) {
  return cv::saturate_cast<C>(at<i>(t));
}

template<std::size_t i, std::size_t j, typename C, typename T>
constexpr
decltype(auto)
at(const T& t) {
  return cv::saturate_cast<C>(at<i, j>(t));
}


/**
 * scalar add operation on opencv types
 */

template<typename T, int m, int n, typename N, VCCC_REQUIRE(std::is_arithmetic<N>::value)>
cv::Matx<T, m, n> add(const cv::Matx<T, m, n>& matx, N n_){
  return matx + cv::Matx<T, m, n>::all(n_);
}

template<typename T, int cn, typename N, VCCC_REQUIRE(std::is_arithmetic<N>::value)>
cv::Vec<T, cn> add(const cv::Vec<T, cn>& vec, N n) {
  return vec + cv::Vec<T, cn>::all(n);
}

template<typename T, typename N, VCCC_REQUIRE(std::is_arithmetic<N>::value)>
cv::Point_<T> add(const cv::Point_<T>& point2, N n) {
  return cv::Point_<T>(point2.x + n, point2.y + n);
}

template<typename T, typename N, VCCC_REQUIRE(std::is_arithmetic<N>::value)>
cv::Point3_<T> add(const cv::Point3_<T>& point3, N n) {
  return cv::Point3_<T>(point3.x + n, point3.y + n, point3.z + n);
}

}

#endif //VCCC_TYPE_SUPPORT_AT_HPP
