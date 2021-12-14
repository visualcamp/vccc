# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_AT_HPP
# define VCCC_TYPE_SUPPORT_AT_HPP
#
# include "vccc/type_support/detail/container_at.hpp"
# include "vccc/type_support/core.hpp"
# include "vccc/type_support/cast.hpp"
# include "vccc/type_traits.hpp"
#
# /*
#  * Note
#  * Specializing std::get is implemented in each class' headers
#  */

namespace vccc{

/**
@addtogroup type_support
@{
    @defgroup type_support_at vccc::at
    @brief Index-based value accessor
@}
*/

//! @addtogroup type_support_at
//! @{

/**
 * @brief returns std::get<I>(t)
 *
 * @tparam I
 * @tparam T
 * @param t
 * @return
 */
template<std::size_t I, typename T, std::enable_if_t<detail::is_tuple_like<std::decay_t<T>>::value, int> = 0>
constexpr decltype(auto) at(T&& t) noexcept {
  return std::get<I>(std::forward<T>(t));
}

/**
@brief returns cv::saturate_cast<C>(vccc::at<i>(t))

@tparam i   index
@tparam C   new type
@param t    param
@return     cv::saturate_cast<C>(at<i>(t))
 */
template<std::size_t i, typename C, typename T, std::enable_if_t<detail::is_tuple_like<std::decay_t<T>>::value, int> = 0>
constexpr inline decltype(auto) at(T&& t) {
  return cast<C>(at<i>(std::forward<T>(t)));
}


/**
@brief returns cv::saturate_cast<C>(vccc::at<i, j>(t))

@tparam i   1D index
@tparam j   2D index
@tparam C   new type
@param t    param
@return     cv::saturate_cast<C>(at<i, j>(t))
 */
template<std::size_t i, std::size_t j, typename C, typename T, std::enable_if_t<detail::is_tuple_like<std::decay_t<T>>::value, int> = 0>
constexpr inline decltype(auto) at(T&& t) {
  return cast<C>(at<i, j>(std::forward<T>(t)));
}



namespace detail{


//TODO: change tuple element to use reference_wrapper
template<typename ...Ts>
class bind_obj {
 public:
  template<typename ...Args>
  constexpr bind_obj(Args&... args) : tup(args...) {}

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
        (at<I>(tup) = at<I>(type), 0)...
    };
  }


  std::tuple<Ts...> tup;
};

}

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
constexpr inline detail::bind_obj<Args&...> bind_at(Args&... args) {
  return detail::bind_obj<Args&...>(args...);
}

//! @} type_support_at

}

# endif //VCCC_TYPE_SUPPORT_AT_HPP