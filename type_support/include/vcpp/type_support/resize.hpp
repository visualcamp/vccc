//
// Created by YongGyu Lee on 2020/12/02.
//

#ifndef VCPP_TYPE_RESIZE_HPP
#define VCPP_TYPE_RESIZE_HPP

#include "opencv2/core/types.hpp"
#include "vcpp/type_traits.hpp"
#include "vcpp/type_support/convert_to.hpp"

namespace vc{

/**
 * resize opencv types
 */

template<int new_size, typename T, VCPP_REQUIRE(new_size==2)>
auto resize(const cv::Point_<T>& from){
  return from;
}

template<int new_size, typename T, VCPP_REQUIRE(new_size==3)>
auto resize(const cv::Point_<T>& from){
  return convert_to<cv::Point3_<T>>(from);
}

template<int new_size, typename T, VCPP_REQUIRE(new_size==2)>
auto resize(const cv::Point3_<T>& from){
  return convert_to<cv::Point_<T>>(from);
}

template<int new_size, typename T, VCPP_REQUIRE(new_size==3)>
auto resize(const cv::Point3_<T>& from){
  return from;
}

template<int new_size, typename T, int old_size>
auto resize(const cv::Vec<T, old_size>& from){
  return convert_to<cv::Vec<T, new_size>>(from);
}


/** any resizable type */

template<int new_size, typename T, typename = void_t<decltype(std::declval<T>().resize(new_size))>>
decltype(auto) resize(T&& t){
  auto copy_or_same = std::forward<T>(t);
  copy_or_same.resize(new_size);
  return copy_or_same;
}

}

#endif //VCPP_TYPE_RESIZE_HPP
