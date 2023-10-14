# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
#
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "opencv2/opencv.hpp"
#
# include "vccc/type_support/opencv/cv_mat.hpp"

namespace vccc {
namespace internal {

template<size_t I, bool size_check>
struct get_cv_vec;

template<size_t I>
struct get_cv_vec<I, true> {
  template<typename T, int cn> static constexpr       T&  get(      cv::Vec<T, cn>&  v) noexcept { return v[I];            }
  template<typename T, int cn> static constexpr const T&  get(const cv::Vec<T, cn>&  v) noexcept { return v[I];            }
  template<typename T, int cn> static constexpr       T&& get(      cv::Vec<T, cn>&& v) noexcept { return std::move(v[I]); }
  template<typename T, int cn> static constexpr const T&& get(const cv::Vec<T, cn>&& v) noexcept { return std::move(v[I]); }
};

template<size_t I, typename T, int n>
struct tuple_element_impl<I, cv::Vec<T, n>, true> {
  using type = T;
};

} // namespace internal
} // namespace vccc

namespace std {

/**
@addtogroup type_support
@{

@addtogroup type_support_tuple_size std::tuple_size
@{
@addtogroup type_support_tuple_size_cv_vec std::tuple_size<cv::Vec>
@{
*/
template<typename T, int cn>
struct tuple_size<cv::Vec<T, cn>> : std::integral_constant<size_t, cn> {};
//! @} type_support_tuple_size_cv_vec
//! @} type_support_tuple_size

/**
@addtogroup type_support_tuple_element std::tuple_element
@{
@addtogroup type_support_tuple_element_cv_vec std::tuple_element<cv::Vec>
@{
*/
template<size_t I, typename T, int cn>
struct tuple_element<I, cv::Vec<T, cn>> : vccc::internal::tuple_element_impl<I, cv::Vec<T, cn>, (I < cn)> {};
//! @} type_support_tuple_element_cv_vec
//! @} type_support_tuple_element


/**
@addtogroup type_support_get std::get
@{
@addtogroup type_support_at_cv_vec std::get(cv::Vec)
@{
*/
template<std::size_t i, typename T, int n>
constexpr inline tuple_element_t<i, cv::Vec<T, n>>&
get(cv::Vec<T, n>& vec) {
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr inline const tuple_element_t<i, cv::Vec<T, n>>&
get(const cv::Vec<T, n>& vec) {
  return vec[i];
}

template<std::size_t i, typename T, int n>
constexpr inline tuple_element_t<i, cv::Vec<T, n>>&&
get(cv::Vec<T, n>&& vec) {
  return std::move(vec[i]);
}

template<std::size_t i, typename T, int n>
constexpr inline const tuple_element_t<i, cv::Vec<T, n>>&&
get(const cv::Vec<T, n>&& vec) {
  return std::move(vec[i]);
}

//! @} type_support_at_cv_vec
//! @} type_support

} // namespace std

namespace vccc {

/** add */

template<typename T, int cn, typename N>
cv::Vec<T, cn> add(const cv::Vec<T, cn>& vec, N n)
{
  auto copy = vec;
  for(int i=0; i<cn; ++i)
    copy[i] += n;
  return copy;
}

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_OPENCV_CV_VEC_HPP
