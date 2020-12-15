# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
#ifndef VCCC_HIGH_VER_TUPLE_APPLY_HPP
#define VCCC_HIGH_VER_TUPLE_APPLY_HPP
#
#
# if __cplusplus <= 201402L
#
# include "vccc/high_ver/tuple/detail/apply.hpp"

namespace vccc{

template<class F, class Tuple>
constexpr
decltype(auto)
apply(F&& f, Tuple&& t) {
  return detail::apply_impl(
      std::forward<F>(f), std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

}

# else
#
# include <tuple>

namespace vccc{

template<class F, class Tuple>
[[deprecated("use std::apply instead")]]
constexpr
decltype(auto)
apply(F&& f, Tuple&& t) {
  return std::apply(std::forward<F>(f), std::forward<Tuple>(t));
}

}

# endif
#
# endif //VCCC_HIGH_VER_TUPLE_APPLY_HPP
