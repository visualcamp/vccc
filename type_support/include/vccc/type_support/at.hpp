# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_AT_HPP
# define VCCC_TYPE_SUPPORT_AT_HPP
#
# include "vccc/type_support/core.hpp"
# include "vccc/type_support/detail/container_at.hpp"
# include "vccc/type_traits.hpp"
#
# /*
#   Note
#   vc::at is implemented in each class' headers
# */

namespace vc{

/**
 * returns cv::saturate_cast<C>(at<i>(t))
 */

template</* manual */ std::size_t i, typename C,
         /* deduce */ typename T>
constexpr
decltype(auto)
at(const T& t) {
  return cv::saturate_cast<C>(at<i>(t));
}


/**
 * returns cv::saturate_cast<C>(at<i, j>(t))
 */

template</* manual */ std::size_t i, std::size_t j, typename C,
         /* deduce */typename T>
constexpr
decltype(auto)
at(const T& t) {
  return cv::saturate_cast<C>(at<i, j>(t));
}

/**
 *  call at<...>(...) to args...
 */

namespace detail{

template<typename ...Ts>
class bind_obj {
 public:
  template<typename ...Args>
  constexpr bind_obj(Args&&... args) : tup(std::forward<Args>(args)...) {}

  template<typename T>
  constexpr bind_obj& operator = (T&& type) {
    bind_impl(std::forward<T>(type), std::index_sequence_for<Ts...>{});
    return *this;
  }

 private:
  template<typename T, std::size_t ...I>
  constexpr void bind_impl(T&& type, std::index_sequence<I...>) {
    [[maybe_unused]] volatile int dummy[sizeof...(I)] = {
        (at<I>(tup) = at<I>(type), 0)...
    };
  }


  std::tuple<Ts...> tup;
};

}

template<typename ...Args>
constexpr detail::bind_obj<Args...> bind_at(Args&&... args) {
  return detail::bind_obj<Args...>(std::forward<Args>(args)...);
}

/**
 * scalar add operation on opencv types
 */





}

#endif //VCCC_TYPE_SUPPORT_AT_HPP
