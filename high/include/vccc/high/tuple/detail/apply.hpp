# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_HIGH_TUPLE_DETAIL_APPLY_HPP
# define VCCC_HIGH_TUPLE_DETAIL_APPLY_HPP
#
# include "vccc/high/functional.hpp"

namespace vccc { namespace detail {

template<class F, class Tuple, std::size_t... I>
constexpr
decltype(auto)
apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
  return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}

}}

#endif //VCCC_HIGH_TUPLE_DETAIL_APPLY_HPP
