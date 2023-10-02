# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
#
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "opencv2/opencv.hpp"

namespace vccc {
namespace detail {

template<size_t I>
struct get_cv_point;

template<>
struct get_cv_point<0> {
  template<typename T> static constexpr       T&  get(      cv::Point_<T>&  p) noexcept { return p.x;            }
  template<typename T> static constexpr const T&  get(const cv::Point_<T>&  p) noexcept { return p.x;            }
  template<typename T> static constexpr       T&& get(      cv::Point_<T>&& p) noexcept { return std::move(p.x); }
  template<typename T> static constexpr const T&& get(const cv::Point_<T>&& p) noexcept { return std::move(p.x); }

  template<typename T> static constexpr       T&  get(      cv::Point3_<T>&  p) noexcept { return p.x;            }
  template<typename T> static constexpr const T&  get(const cv::Point3_<T>&  p) noexcept { return p.x;            }
  template<typename T> static constexpr       T&& get(      cv::Point3_<T>&& p) noexcept { return std::move(p.x); }
  template<typename T> static constexpr const T&& get(const cv::Point3_<T>&& p) noexcept { return std::move(p.x); }
};

template<>
struct get_cv_point<1> {
  template<typename T> static constexpr       T&  get(      cv::Point_<T>&  p) noexcept { return p.y;            }
  template<typename T> static constexpr const T&  get(const cv::Point_<T>&  p) noexcept { return p.y;            }
  template<typename T> static constexpr       T&& get(      cv::Point_<T>&& p) noexcept { return std::move(p.y); }
  template<typename T> static constexpr const T&& get(const cv::Point_<T>&& p) noexcept { return std::move(p.y); }

  template<typename T> static constexpr       T&  get(      cv::Point3_<T>&  p) noexcept { return p.y;            }
  template<typename T> static constexpr const T&  get(const cv::Point3_<T>&  p) noexcept { return p.y;            }
  template<typename T> static constexpr       T&& get(      cv::Point3_<T>&& p) noexcept { return std::move(p.y); }
  template<typename T> static constexpr const T&& get(const cv::Point3_<T>&& p) noexcept { return std::move(p.y); }
};

template<>
struct get_cv_point<2> {
  template<typename T> static constexpr       T&  get(      cv::Point3_<T>&  p) noexcept { return p.z;            }
  template<typename T> static constexpr const T&  get(const cv::Point3_<T>&  p) noexcept { return p.z;            }
  template<typename T> static constexpr       T&& get(      cv::Point3_<T>&& p) noexcept { return std::move(p.z); }
  template<typename T> static constexpr const T&& get(const cv::Point3_<T>&& p) noexcept { return std::move(p.z); }
};

} // namespace detail
} // namespace vccc

namespace std {

/**
@addtogroup type_support
@{

@addtogroup type_support_tuple_size std::tuple_size
@{
@addtogroup type_support_tuple_size_cv_point std::tuple_size<cv::Point_>
@{
*/
template<typename T>
struct tuple_size<cv::Point_<T>> : std::integral_constant<size_t, 2> {};
//! @} type_support_tuple_size_cv_point

/**
@addtogroup type_support_tuple_size_cv_point3 std::tuple_size<cv::Point3_>
@{
*/
template<typename T>
struct tuple_size<cv::Point3_<T>> : std::integral_constant<size_t, 3> {};
//! @} type_support_tuple_size_cv_point3

//! @} type_support_tuple_size


/**
@addtogroup type_support_tuple_element std::tuple_element
@{

@addtogroup type_support_tuple_element_cv_point std::tuple_element<cv::Point_>
@{
*/
template<typename T> struct tuple_element<0, cv::Point_<T>> { using type = T; };
template<typename T> struct tuple_element<1, cv::Point_<T>> { using type = T; };
//! @} type_support_tuple_element_cv_point


/**
@addtogroup type_support_tuple_element_cv_point3 std::tuple_element<cv::Point3_>
@{
*/
template<typename T> struct tuple_element<0, cv::Point3_<T>> { using type = T; };
template<typename T> struct tuple_element<1, cv::Point3_<T>> { using type = T; };
template<typename T> struct tuple_element<2, cv::Point3_<T>> { using type = T; };
//! @} type_support_tuple_element_cv_point3
//! @} type_support_tuple_element




/**
@addtogroup type_support_get std::get
@{

@addtogroup type_support_at_cv_point std::get(cv::Point_)
@{
*/
template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Point_<T>>&
get(cv::Point_<T>& p) noexcept {
  return vccc::detail::get_cv_point<i>::get(p);
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Point_<T>>&
get(const cv::Point_<T>& p) noexcept {
  return vccc::detail::get_cv_point<i>::get(p);
}

template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Point_<T>>&&
get(cv::Point_<T>&& p) noexcept {
  return std::move(vccc::detail::get_cv_point<i>::get(std::move(p)));
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Point_<T>>&&
get(const cv::Point_<T>&& p) noexcept {
  return std::move(vccc::detail::get_cv_point<i>::get(std::move(p)));
}
//! @} type_support_at_cv_point


/**
@addtogroup type_support_at_cv_point3 std::get(cv::Point3_)
@{
*/
template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Point3_<T>>&
get(cv::Point3_<T>& p) noexcept {
  return vccc::detail::get_cv_point<i>::get(p);
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Point3_<T>>&
get(const cv::Point3_<T>& p) noexcept {
  return vccc::detail::get_cv_point<i>::get(p);
}

template<std::size_t i, typename T>
constexpr inline tuple_element_t<i, cv::Point3_<T>>&&
get(cv::Point3_<T>&& p) noexcept {
  return std::move(vccc::detail::get_cv_point<i>::get(std::move(p)));
}

template<std::size_t i, typename T>
constexpr inline const tuple_element_t<i, cv::Point3_<T>>&&
get(const cv::Point3_<T>&& p) noexcept {
  return std::move(vccc::detail::get_cv_point<i>::get(std::move(p)));
}
//! @} type_support_at_cv_point3
//! @} type_support_get
//! @} type_support

} // namespace std

namespace vccc {

/** add */

template<typename T, typename N>
cv::Point_<T> add(const cv::Point_<T>& point2, N n) {
  return cv::Point_<T>(point2.x + n, point2.y + n);
}

template<typename T, typename N>
inline cv::Point3_<T> add(const cv::Point3_<T>& point3, N n) {
  return cv::Point3_<T>(point3.x + n, point3.y + n, point3.z + n);
}

/** resize */

template<int new_size, typename T, std::enable_if_t<new_size == 2, int> = 0>
auto resize(const cv::Point_<T>& from) {
  return from;
}

template<int new_size, typename T, std::enable_if_t<new_size==3, int> = 0>
auto resize(const cv::Point_<T>& from) {
  return cv::Point3_<T>(from.x, from.y, 0);
}

template<int new_size, typename T, VCCC_ENABLE_IF(new_size==2)>
inline auto resize(const cv::Point3_<T>& from){
  return cv::Point_<T>(from.x, from.y);
}

template<int new_size, typename T, VCCC_ENABLE_IF(new_size==3)>
inline auto resize(const cv::Point3_<T>& from){
  return from;
}


} // namespace vccc

# endif //VCCC_TYPE_SUPPORT_OPENCV_CV_POINT_HPP
