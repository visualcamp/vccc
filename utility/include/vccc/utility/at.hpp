//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_UTILITY_AT_HPP
#define VCCC_UTILITY_AT_HPP

#include <cstddef>
#include <tuple>
#include <iterator>

#include "vccc/type_traits.hpp"

namespace vc{


/** tuple-like */

template<std::size_t i, typename T,
    VCCC_REQUIRE(i < std::tuple_size<T>::value)>
constexpr
decltype(auto) at (T& tuple) {
  return std::get<i>(tuple);
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i < std::tuple_size<T>::value)>
constexpr
decltype(auto) at (const T& tuple) {
  return std::get<i>(tuple);
}

template<std::size_t i, typename ...Args,
    VCCC_REQUIRE(i < sizeof...(Args))>
constexpr
decltype(auto) at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

/** container */

template<std::size_t i, typename Container,
VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto) at(Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container,
VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto) at(const Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}


/** std::array */

template<std::size_t i, typename T, int n,
    VCCC_REQUIRE(i < n)>
constexpr
decltype(auto) at(std::array<T, n>& arr) {
  return arr[i];
}

template<std::size_t i, typename T, int n,
    VCCC_REQUIRE(i < n)>
constexpr
decltype(auto) at(const std::array<T, n>& arr) {
  return arr[i];
}

/** opencv types */
#ifdef OPENCV_CORE_TYPES_HPP


/** cv::Point_ */

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

#else

/**
 * return static_cast<C>(at<i,...>(t))
 */
template<std::size_t i,                typename C, typename T> auto constexpr at(const T& t){return static_cast<C>(at<i   >(t));}
template<std::size_t i, std::size_t j, typename C, typename T> auto constexpr at(const T& t){return static_cast<C>(at<i, j>(t));}

#endif

/**
 *  call at<...>(t) to args...
 */

namespace detail{

template<typename ...Ts>
class bind_obj {
 public:
  template<typename ...Args>
  constexpr bind_obj(Args&&... args) : tup(std::forward<Args>(args)...) {}

  template<typename T>
  constexpr bind_obj& operator = (T&& type) {
    bind_impl(std::forward<T>(type), std::index_sequence_for<Ts...>{});
    return *this;
  }

 private:
  template<typename T, std::size_t ...I>
  constexpr void bind_impl(T&& type, std::index_sequence<I...>) {
    [[maybe_unused]] volatile int dummy[sizeof...(I)] = {
        (at<I>(tup) = at<I>(type), 0)...
    };
  }


  std::tuple<Ts...> tup;
};

}

template<typename ...Args>
constexpr detail::bind_obj<Args...> bind_at(Args&&... args) {
  return detail::bind_obj<Args...>(std::forward<Args>(args)...);
}

}

#endif //VCCC_UTILITY_AT_HPP
