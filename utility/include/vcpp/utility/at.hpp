//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_UTILITY_AT_HPP
#define VCPP_UTILITY_AT_HPP

#include <cstddef>
#include <tuple>
#include <iterator>

#include "vcpp/type_traits.hpp"

namespace vc{


/** tuple-like */

template<std::size_t i, typename T,
    VCPP_REQUIRE(i < std::tuple_size<T>::value)>
decltype(auto) at (T& tuple) {
  return std::get<i>(tuple);
}

template<std::size_t i, typename T,
    VCPP_REQUIRE(i < std::tuple_size<T>::value)>
decltype(auto) at (const T& tuple) {
  return std::get<i>(tuple);
}


/** container */

template<std::size_t i, typename Container,
    VCPP_REQUIRE(is_container_v<Container>)>
decltype(auto) at(Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container,
    VCPP_REQUIRE(is_container_v<Container>)>
decltype(auto) at(const Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}


/** std::array */

template<std::size_t i, typename T, int n,
    VCPP_REQUIRE(i < n)>
decltype(auto) at(std::array<T, n>& arr) {
  return arr[i];
}

template<std::size_t i, typename T, int n,
    VCPP_REQUIRE(i < n)>
constexpr decltype(auto) at(const std::array<T, n>& arr) {
  return arr[i];
}

/** opencv types */
#ifdef OPENCV_CORE_TYPES_HPP


/** cv::Point_ */

template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)>       T& at(      cv::Point_<T>& point2) { return point2.x; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)> const T& at(const cv::Point_<T>& point2) { return point2.x; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)>       T& at(      cv::Point_<T>& point2) { return point2.y; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)> const T& at(const cv::Point_<T>& point2) { return point2.y; }


/** cv::Point3_ */

template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)>       T& at(      cv::Point3_<T>& point3) { return point3.x; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)> const T& at(const cv::Point3_<T>& point3) { return point3.x; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)>       T& at(      cv::Point3_<T>& point3) { return point3.y; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)> const T& at(const cv::Point3_<T>& point3) { return point3.y; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 2)>       T& at(      cv::Point3_<T>& point3) { return point3.z; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 2)> const T& at(const cv::Point3_<T>& point3) { return point3.z; }


/** cv::Vec */

template<std::size_t i, typename T, int n, VCPP_REQUIRE(i < n)> decltype(auto) at(      cv::Vec<T, n>& vec) { return vec[i]; }
template<std::size_t i, typename T, int n, VCPP_REQUIRE(i < n)> decltype(auto) at(const cv::Vec<T, n>& vec) { return vec[i]; }


/** cv::Matx */

template<std::size_t row, std::size_t col, typename T, int m, int n,
         VCPP_REQUIRE((row < m && col < n) && (m > 1 && n > 1))>
decltype(auto) at(      cv::Matx<T, m, n>& matx) { return matx(row, col); }

template<std::size_t row, std::size_t col, typename T, int m, int n,
         VCPP_REQUIRE((row < m && col < n) && (m > 1 && n > 1))>
decltype(auto) at(const cv::Matx<T, m, n>& matx) { return matx(row, col); }

template<std::size_t i, typename T, int m, int n,
    VCPP_REQUIRE((i < m * n) && (m == 1 || n == 1))>
decltype(auto) at(      cv::Matx<T, m, n>& matx) { return matx(i); }

template<std::size_t i, typename T, int m, int n,
    VCPP_REQUIRE((i < m * n) && (m == 1 || n == 1))>
decltype(auto) at(const cv::Matx<T, m, n>& matx) { return matx(i); }


/** cv::Size_ */

template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)>       T& at(      cv::Size_<T>& size) { return size.width; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 0)> const T& at(const cv::Size_<T>& size) { return size.width; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)>       T& at(      cv::Size_<T>& size) { return size.height; }
template<std::size_t i, typename T, VCPP_REQUIRE(i == 1)> const T& at(const cv::Size_<T>& size) { return size.height; }

/**
 * return cv::saturate_cast<C>(at<i,...>(t))
 */
template<std::size_t i,                typename C, typename T> auto constexpr at(const T& t){return cv::saturate_cast<C>(at<i   >(t));}
template<std::size_t i, std::size_t j, typename C, typename T> auto constexpr at(const T& t){return cv::saturate_cast<C>(at<i, j>(t));}


/**
 * scalar add operation on opencv types
 */

template<typename T, int m, int n, typename N, require(std::is_arithmetic<N>::value)>
cv::Matx<T, m, n> add(const cv::Matx<T, m, n>& matx, T _n){
  return matx + cv::Matx<T, m, n>::all(_n);
}

template<typename T, int cn, typename N, require(std::is_arithmetic<N>::value)>
cv::Vec<T, cn> add(const cv::Vec<T, cn>& vec, N n) {
  return vec + cv::Vec<T, cn>::all(n);
}

template<typename T, typename N, require(std::is_arithmetic<N>::value)>
cv::Point_<T> add(const cv::Point_<T>& point2, N n) {
  return cv::Point_<T>(point2.x + n, point2.y + n);
}

template<typename T, typename N, require(std::is_arithmetic<N>::value)>
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
  constexpr void operator = (T&& type) {
    bind_impl(std::forward<T>(type), std::index_sequence_for<Ts...>{});
  }

 private:
  template<typename T, std::size_t ...I>
  constexpr void bind_impl(T&& type, std::index_sequence<I...>) {
    volatile int dummy[sizeof...(I)] = {
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

#endif //VCPP_UTILITY_AT_HPP
