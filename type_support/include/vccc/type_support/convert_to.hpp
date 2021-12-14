# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_CONVERT_HPP
# define VCCC_TYPE_SUPPORT_CONVERT_HPP
#
# include "vccc/type_support/detail/convert_to.hpp"

namespace vccc {

/**
@addtogroup type_support
@{
    @defgroup type_support_cvtto vccc::convert_to
    @brief converts to different type (usually for opencv types, but also supports container & tuple-like types)
@}

*/

/**
@addtogroup type_support_cvtto
@{

!Warning! if input container size is smaller then R's size, it's ub.

Conversion rule:  
      1. If `std::is_convertible<From, To>::value == true`  
         Construct `To` with `std::forward<From>(from)`  
         
      Else, `cv::saturate_cast` is used in every element-wise conversion.  
      Suppose `T = std::tuple_size<To>::value`, and `F = std::tuple_size<From>::value`.  
      
      2. Converting tuple-like type to tuple-like type  
         a. `T <= F`  
             Construct To with `std::get<Ti>(From)...`  
         b. `T > F`  
             Convert all From's element, remainders will be zero-initialized.  

      3. Converting tuple-like type to range type(see @ref is_range)  
         Creates a container that contains all From's elements.  

      4. Converting range type to tuple-like type  
         a. `range.size() >= T`  
             Convert only `F` amount of container's element. Remainders will be zero-initialized.  
         c. `range.size() < T`  
             !!- undefined behaviour -!!  

      5. Converting range to range
         * Not supported

@tparam To   to type
@param from  original data
@return      converted class

@code
      cv::Vec3d v = {1.1, 2.4, 3.6};
      auto vec = convert_to<std::vector<int>>(v); // vec = {1, 2. 4}
      auto point = convert_to<cv::Point2f>(v);    // point = {1.1, 2.4}
      auto v2 = convert_to<cv::Vec4d>(point);     // v2 = {1.1, 2.4, 0, 0}

      point = convert_to<cv::Point2f>(vec);       // point = {1, 2}
      // v2 = convert_to<cv::Vec4d>(vec); undefined behaviour! vec.size() < cv_size_v<Vec4d>
@endcode
*/

//template<typename To, typename From,
//    std::enable_if_t<
//      detail::check_convert_to<const From&, To>::value,
//    int> = 0>
//constexpr To
//convert_to(const From& from);

/**
@brief convert with specifying the converting size
 * convert to container types: same size as n
 * convert to opencv types: fill rest with 0

@tparam To       result type
@tparam n        converting size
@param from      original data
@return          converted data
 */

template<typename To, std::size_t n, typename From,
  std::enable_if_t<
    detail::check_convert_to<From&&, To>::value,
  int> = 0>
constexpr To
convert_to(From& from) {
  return detail::convert_to_impl<To>(
    std::is_convertible<From&&, To>{},
    conjunction<detail::is_tuple_like<std::decay_t<To>>, detail::is_tuple_like<std::decay_t<From>>>{},
    std::forward<From>(from),
    std::make_index_sequence<n>{});
}


template<typename To, typename From,
  std::enable_if_t<
    detail::check_convert_to<From&&, To>::value,
  int> = 0>
constexpr To
convert_to(From&& from) {
//  return convert_to<To, detail::check_convert_to<From&&, To>::converting_size>(std::forward<From>(from));
  return detail::convert_to_impl<To>(
    std::is_convertible<const From&, To>{},
    conjunction<detail::is_tuple_like<std::decay_t<To>>, detail::is_tuple_like<std::decay_t<From>>>{},
    std::forward<From>(from),
    std::make_index_sequence<detail::check_convert_to<From&&, To>::converting_size>{});
}

//! @} type_support_cvtto

}

# endif //VCCC_TYPE_SUPPORT_CONVERT_HPP
