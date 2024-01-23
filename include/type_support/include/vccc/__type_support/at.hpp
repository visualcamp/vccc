# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_AT_HPP
# define VCCC_TYPE_SUPPORT_AT_HPP
#
# include "vccc/__type_support/detail/container_at.hpp"
# include "vccc/__type_support/core.hpp"
# include "vccc/__type_support/cast.hpp"
# include "vccc/type_traits.hpp"
#
# /*
#  * Note
#  * vccc::at is implemented in each class' headers
#  */

namespace vccc {

/**
 * @addtogroup type_support
 * @{
 * @defgroup type_support_at__func at
 * @brief extended version of std::get
 * @defgroup type_support_get__func std::get
 * @brief extended std::get
 * @}
 */

/**
 * @addtogroup type_support
 * @{
 * @addtogroup type_support_at__func
 * @{
 */

/**
 * @brief Return i-th element of tuple-like object
 *
 * returns `std::get<I>(t)`
 *
 * @tparam I index
 */

template<std::size_t I, typename T, std::enable_if_t<is_tuple_like<std::decay_t<T>>::value, int> = 0>
constexpr decltype(auto) at(T&& t) noexcept {
  return std::get<I>(std::forward<T>(t));
}

/**
 * @brief returns `cv::saturate_cast<C>(vccc::at<i>(t))`
 */
template<std::size_t i, typename C, typename T>
constexpr decltype(auto) at(T&& t) {
  return cast<C>(at<i>(std::forward<T>(t)));
}


/**
 * @brief returns cv::saturate_cast<C>(vccc::at<i, j>(t))
 */
template<std::size_t i, std::size_t j, typename C, typename T>
constexpr decltype(auto) at(T&& t) {
  return cast<C>(at<i, j>(std::forward<T>(t)));
}

//! @}


namespace internal {

//TODO: change tuple element to use reference_wrapper
template<typename ...Ts>
class bind_obj {
 public:
  template<typename ...Args>
  constexpr bind_obj(Args&&... args) : tup(std::forward<Args>(args)...) {}

  template<typename T>
  constexpr bind_obj& operator = (T&& type)
  {
    bind_impl(std::forward<T>(type), std::index_sequence_for<Ts...>{});
    return *this;
  }

 private:
  template<typename T, std::size_t ...I>
  constexpr void bind_impl(T&& type, std::index_sequence<I...>)
  {
    int dummy[sizeof...(I)] = {
        (at<I>(tup) = at<I>(std::forward<T>(type)), 0)...
    };
  }


  std::tuple<Ts...> tup;
};

} // namespace internal

/**
@brief call at<...>(...) to args...

@code
float x, y;
cv::Point2f p(1.1f, 2.2f);

vccc::bind_at(x, y) = p;
@endcode

@param ...args args
@return helper class bind_obj
 */

template<typename ...Args>
constexpr inline internal::bind_obj<Args...> bind_at(Args&&... args) {
  return internal::bind_obj<Args...>(std::forward<Args>(args)...);
}

//! @}

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_AT_HPP
