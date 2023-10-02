# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
#
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "opencv2/opencv.hpp"
#
# include "vccc/type_traits.hpp"

namespace vccc {
namespace detail {

template<size_t I>
struct get_cv_size;

template<>
struct get_cv_size<0> {
  template<typename T> static constexpr       T&  get(      cv::Size_<T>& s ) noexcept { return s.width;            }
  template<typename T> static constexpr const T&  get(const cv::Size_<T>& s ) noexcept { return s.width;            }
  template<typename T> static constexpr       T&& get(      cv::Size_<T>&& s) noexcept { return std::move(s.width); }
  template<typename T> static constexpr const T&& get(const cv::Size_<T>&& s) noexcept { return std::move(s.width); }
};

template<>
struct get_cv_size<1> {
  template<typename T> static constexpr       T&  get(      cv::Size_<T>& s ) noexcept { return s.height;            }
  template<typename T> static constexpr const T&  get(const cv::Size_<T>& s ) noexcept { return s.height;            }
  template<typename T> static constexpr       T&& get(      cv::Size_<T>&& s) noexcept { return std::move(s.height); }
  template<typename T> static constexpr const T&& get(const cv::Size_<T>&& s) noexcept { return std::move(s.height); }
};

} // namespace detail
} // namespace vccc

namespace std {

/**
@addtogroup type_support
@{

@addtogroup type_support_tuple_size std::tuple_size
@{
@addtogroup type_support_tuple_size_cv_size std::tuple_size<cv::Size_>
@{
*/
template<typename T>
struct tuple_size<cv::Size_<T>> : std::integral_constant<size_t, 2> {};
//! @} type_support_tuple_size_cv_size
//! @} type_support_tuple_size

/**
@addtogroup type_support_tuple_element std::tuple_element
@{
 
@addtogroup type_support_tuple_element_cv_size std::tuple_element<cv::Size_>
@{
*/

template<typename T> struct tuple_element<0, cv::Size_<T>> { using type = T; };
template<typename T> struct tuple_element<1, cv::Size_<T>> { using type = T; };

//! @} type_support_tuple_element_cv_size
//! @} type_support_tuple_element

/**
@addtogroup type_support_get std::get
@{
@addtogroup type_support_get_cv_size std::get(cv::Size_)
@{
*/

template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Size_<T>>&
get(cv::Size_<T>& size) noexcept {
  return vccc::detail::get_cv_size<i>::get(size);
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Size_<T>>&
get(const cv::Size_<T>& size) noexcept {
  return size.width;
}

template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Size_<T>>&&
get(cv::Size_<T>&& size) noexcept {
  return std::move(vccc::detail::get_cv_size<i>::get(std::move(size)));
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Size_<T>>&&
get(const cv::Size_<T>&& size) noexcept {
  return std::move(vccc::detail::get_cv_size<i>::get(std::move(size)));
}

//! @} type_support_get_cv_size
//! @} type_support_get
//! @} type_support

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_SIZE_HPP
