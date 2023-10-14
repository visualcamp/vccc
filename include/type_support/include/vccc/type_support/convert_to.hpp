# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_CONVERT_HPP
# define VCCC_TYPE_SUPPORT_CONVERT_HPP
#
# include "vccc/type_support/detail/convert_to.hpp"
# include "vccc/type_support/opencv/core.hpp"

namespace vccc {

/**
@addtogroup type_support
@{
    @defgroup type_support_cvtto vccc::convert_to
    @brief converts to different type (usually for opencv types, but also supports container types)
@}

*/

/**
@addtogroup type_support_cvtto
@{

!Warning! If input container size is smaller then R's size, it's ub.
!Warning! If OpenCV is enabled, arithmetic floating-to-integer will be rounded(e.g 3.6 -> 4)

conversion rule:
  * cv::saturate_cast is used in every element-wise conversion
      1. converting tuple-like types to tuple-like types
         a. tuple_size<To>::value == tuple_size<From>::value
             convert all elements.
         b. tuple_size<To>::value < tuple_size<From>::value
             convert only tuple_size<To>::value amount of From's element. Remainders won't convert
         c. tuple_size<To>::value > tuple_size<From>::value
             convert all From's element, and fill rest with zeros
      2. converting tuple-like types to container-types
         creates a container that contains all From's elements
      3. converting container-types to tuple-like types
         a. tuple_size<To>::value == container.size()
             converts every element
         b. tuple_size<To>::value < container.size()
             convert only tuple_size<To>::value amount of container's element. Remainders won't convert
         c. tuple_size<To>::value > container.size()
             !!- undefined behaviour -!!
      4. converting container-types to container-types
         * not supported
@tparam To   to type
@param from  original data
@return      converted class

@code
      cv::Vec3d v = {1.1, 2.4, 3.6};
      auto vec = convert_to<std::vector<int>>(v); // vec = {1, 2. 4}
      auto point = convert_to<cv::Point2f>(v);    // point = {1.1, 2.4}
      auto v2 = convert_to<cv::Vec4d>(point);     // v2 = {1.1, 2.4, 0, 0}

      point = convert_to<cv::Point2f>(vec);       // point = {1, 2}
      // v2 = convert_to<cv::Vec4d>(vec); undefined behaviour! vec.size() < tuple_size<Vec4d>::value
@endcode
*/

template<typename To, typename From, std::enable_if_t<!std::is_same<To, From>::value, int> = 0>
inline std::enable_if_t<disjunction<is_tuple_like<To>, is_tuple_like<From>>::value, To>
convert_to(const From& from)
{
  const auto v1 = is_tuple_like<To>::value;
  const auto v2 = is_tuple_like<From>::value;
  const auto v3 = internal::tuple_size_or_zero<To>::value;
  const auto v4 = internal::tuple_size_or_zero<From>::value;
  using Indices = typename std::make_index_sequence<
      (is_tuple_like<To>::value && is_tuple_like<From>::value) ? static_min<size_t, internal::tuple_size_or_zero<To>::value, internal::tuple_size_or_zero<From>::value>::value :
                                   is_tuple_like<From>::value  ? internal::tuple_size_or_zero<From>::value :
                                   internal::tuple_size_or_zero<To>::value>;
  return internal::convert_to_impl<To>(is_tuple_like<To>(), from, Indices{});
}

/**
@brief convert with specifying the converting size
 * convert to container types: same size as n
 * convert to opencv types: fill rest with 0

@tparam To       result type
@tparam n        converting size
@param from      original data
@return          converted data
 */

template<typename To, std::size_t n, typename From, std::enable_if_t<!std::is_same<To, From>::value, int> = 0>
inline std::enable_if_t<disjunction<is_tuple_like<To>, is_tuple_like<From>>::value, To>
convert_to(const From& from)
{
  static_assert(is_tuple_like<To>::value   ? n <= internal::tuple_size_or_zero<To>::value   : true,
      "Converting size must be smaller than converting type's tuple_size");
  static_assert(is_tuple_like<From>::value ? n <= internal::tuple_size_or_zero<From>::value : true,
      "Converting size must be smaller than original type's tuple_size");

  return internal::convert_to_impl<To>(is_tuple_like<To>{}, from, std::make_index_sequence<n>{});
}

/**
@brief converting to same type
 */
template<typename To> inline decltype(auto) convert_to(      To&& from) { return std::forward<To>(from); }
template<typename To> inline To             convert_to(const To&  from) { return from; }

//! @} type_support_cvtto

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_CONVERT_HPP
