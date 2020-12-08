# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
#ifndef VCCC_TYPE_SUPPORT_CONVERT_HPP
#define VCCC_TYPE_SUPPORT_CONVERT_HPP

#include "vccc/type_support/detail/convert_to.hpp"

namespace vc {

/**
 * converts From to To (usually for opencv types, but also supports container types)
 * !Warning! if input container size is smaller then R's size, it's ub.
 *
 * conversion rule:
 *   * cv::saturate_cast is used in every element-wise conversion
 *
 *      1. converting opencv-types to opencv-types
 *          a. cv_size_v<To> == cv_size_v<From>
 *              convert all elements.
 *          b. cv_size_v<To> < cv_size_v<From>
 *              convert only cv_size_v<To> amount of From's element. Remainders won't convert
 *          c. cv_size_v<To> > cv_size_v<From>
 *              convert all From's element, and fill rest with zeros
 *
 *      2. converting opencv-types to container-types(constructable with std::initializer_list)
 *          creates a container that contains all From's elements
 *
 *      3. converting container-types to opencv-types
 *          a. cv_size_v<To> == container.size()
 *              converts every element
 *          b. cv_size_v<To> < container.size()
 *              convert only cv_size_v<To> amount of container's element. Remainders won't convert
 *          c. cv_size_v<To> > container.size()
 *              !!- undefined behaviour -!!
 *
 *      4. converting container-types to container-types
 *          * not supported
 *
 * @tparam To   to type
 * @param From  from type
 * @return      converted class
 *
 *
 * ex)
 *      cv::Vec3d v = {1.1, 2.4, 3.6};
 *      auto vec = convert_to<std::vector<int>>(v); // vec = {1, 2. 4}
 *      auto point = convert_to<cv::Point2f>(v);    // point = {1.1, 2.4}
 *      auto v2 = convert_to<cv::Vec4d>(point);     // v2 = {1.1, 2.4, 0, 0}
 *
 *      point = convert_to<cv::Point2f>(vec);       // point = {1, 2}
 *      // v2 = convert_to<cv::Vec4d>(vec); undefined behaviour! vec.size() < cv_size_v<Vec4d>
 *
 */
template<typename To, typename From, VCCC_REQUIRE((!std::is_same<To, From>::value))>
To convert_to(const From& from){
  static_assert(detail::is_cv_type_v<To> || detail::is_cv_type_v<From>, "You cannot convert non-cv type to non-cv type! use vtype_convert instead");
  using Indices = typename std::make_index_sequence<
      ((detail::is_cv_type_v<To> && detail::is_cv_type_v<From>) ? detail::min_cv_size_v<To, From> :
       (detail::is_cv_type_v<From> ? detail::cv_size_v<From> : detail::cv_size_v<To>))>;
  return detail::convert_to_impl<To>(detail::is_cv_type<To>(), from, Indices{});
}

/**
 * specify the converting size
 *  * convert to container types: same size as n
 *  * convert to opencv types: fill rest with 0
 *
 * @tparam To       result type
 * @tparam n        converting size
 * @tparam From     deduced
 * @param from      original data
 * @return          converted data
 */

template<typename To, std::size_t n, typename From, VCCC_REQUIRE((!std::is_same<To, From>::value))>
To convert_to(const From& from){
  static_assert(detail::is_cv_type_v<To> || detail::is_cv_type_v<From>, "You cannot convert non-cv type to non-cv type! use vtype_convert instead");
  static_assert(detail::is_cv_type_v<To>   ? n <= detail::cv_size_v<To>   : true, "Converting size must be smaller than converting type's cv_size");
  static_assert(detail::is_cv_type_v<From> ? n <= detail::cv_size_v<From> : true, "Converting size must be smaller than original type's cv_size");

  return detail::convert_to_impl<To>(detail::is_cv_type<To>(), from, std::make_index_sequence<n>{});
}

/**
 * converting to same type
 */
template<typename To> decltype(auto) convert_to(      To&& from) { return from; }
template<typename To>             To convert_to(const To&  from) { return from; }

}

#endif //VCCC_TYPE_SUPPORT_CONVERT_HPP
