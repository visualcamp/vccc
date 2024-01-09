# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
#
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "opencv2/opencv.hpp"

namespace vccc {
namespace internal {

template<size_t I, typename CVMatX, bool v>
struct tuple_element_impl;

template<size_t I, typename T, int m, int n>
struct tuple_element_impl<I, cv::Matx<T, m, n>, true> {
  using type = T;
};

} // namespace internal
} // namespace vccc

namespace std {

//template<typename T, int m, int n> struct internal::cv_size<cv::Matx<T, m, n>>;
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T& at(cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T& at(const cv::Matx<T, m, n>& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr T&& at(cv::Matx<T, m, n>&& matx);
//template<std::size_t i, std::size_t j, typename T, int m, int n> constexpr const T&& at(const cv::Matx<T, m, n>&& matx);

/**
@addtogroup type_support
@{

@addtogroup type_support_tuple_size__class std::tuple_size
@{
@addtogroup type_support_tuple_size_cv_matx__class std::tuple_size<cv::Matx>
@{
*/
template<typename T, int m, int n>
struct tuple_size<cv::Matx<T, m, n>> : std::integral_constant<size_t, m * n> {};
//! @}
//! @}

/**
@addtogroup type_support_tuple_element__class std::tuple_element
@{
@addtogroup type_support_tuple_element_cv_matx__class std::tuple_element<cv::Matx>
@{
*/
template<size_t I, typename T, int m, int n>
struct tuple_element<I, cv::Matx<T, m, n>> :
    vccc::internal::tuple_element_impl<I, cv::Matx<T, m, n>, (I < m * n)> {};
//! @}
//! @}



/**
@addtogroup type_support_get__func
@{

@addtogroup type_support_get_cv_matx__func std::get(cv::Matx)
@{
*/
template<std::size_t i, typename T, int m, int n>
constexpr inline tuple_element_t<i, cv::Matx<T, m, n>>&
get(cv::Matx<T, m, n>& matx) noexcept {
  return matx(i);
}

template<std::size_t i, typename T, int m, int n>
constexpr inline const tuple_element_t<i, cv::Matx<T, m, n>>&
get(const cv::Matx<T, m, n>& matx) noexcept {
  return matx(i);
}

template<std::size_t i, typename T, int m, int n>
constexpr inline tuple_element_t<i, cv::Matx<T, m, n>>&&
get(cv::Matx<T, m, n>&& matx) noexcept {
  return std::move(matx(i));
}

template<std::size_t i, typename T, int m, int n>
constexpr inline const tuple_element_t<i, cv::Matx<T, m, n>>&&
get(const cv::Matx<T, m, n>&& matx) noexcept {
  return std::move(matx(i));
}

//! @}
//! @}
//! @} type_support

} // namespace std

namespace vccc {
/**
@addtogroup type_support_at__func
@{
    @defgroup type_support_at_cv_matx__func vccc::at(cv::Matx)
    Index-based value accessor
@}

@addtogroup type_support_at_cv_matx__func
@{
*/

template<std::size_t i, std::size_t j, typename T, int m, int n>
constexpr inline T&
at(cv::Matx<T, m, n>& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j, typename T, int m, int n>
constexpr inline const T&
at(const cv::Matx<T, m, n>& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (const cv::Matx)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx)");
  return matx(i, j);
}

template<std::size_t i, std::size_t j, typename T, int m, int n>
constexpr inline T&&
at(cv::Matx<T, m, n>&& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (cv::Matx&&)");
  return std::move(matx(i, j));
}

template<std::size_t i, std::size_t j, typename T, int m, int n>
constexpr inline const T&&
at(const cv::Matx<T, m, n>&& matx) {
  static_assert(i < m && j < n, "Index out of bounds in vccc::at<> (const cv::Matx&&)");
  static_assert(m > 1 && n > 1, "Invalid size (const cv::Matx&&)");
  return std::move(matx(i, j));
}

//! @}

/** add */

template<typename T, int m, int n, typename N>
cv::Matx<T, m, n> add(const cv::Matx<T, m, n>& matx, N n_)
{
  return matx + cv::Matx<T, m, n>::all(n_);
}

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_OPENCV_CV_MAT_HPP
