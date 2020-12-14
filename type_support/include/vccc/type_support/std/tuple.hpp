# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_TUPLE_HPP
# define VCCC_TYPE_SUPPORT_STD_TUPLE_HPP
#
# include <tuple>

namespace vccc{

template</* manual */ std::size_t i,
         /* deduce */ typename T,
         /*  cond  */ std::enable_if_t<(i < std::tuple_size<T>::value), int> = 0>
constexpr
decltype(auto)
at (T& tup) {
  return std::get<i>(tup);
}

template</* manual */ std::size_t i,
         /* deduce */ typename T,
         /*  cond  */ std::enable_if_t<(i < std::tuple_size<T>::value), int> = 0>
constexpr
decltype(auto)
at (const T& tup) {
  return std::get<i>(tup);
}

template</* manual */ std::size_t i,
         /* deduce */ typename T,
         /*  cond  */ std::enable_if_t<(i < std::tuple_size<T>::value), int> = 0>
constexpr
decltype(auto)
at (T&& tup) {
  return std::get<i>(std::forward<T>(tup));
}

template</* manual */ std::size_t i,
         /* deduce */ typename T,
         /*  cond  */ std::enable_if_t<(i < std::tuple_size<T>::value), int> = 0>
constexpr
decltype(auto)
at (const T&& tup) {
  return std::get<i>(std::forward<T>(tup));
}

}

# endif //VCCC_TYPE_SUPPORT_STD_TUPLE_HPP
