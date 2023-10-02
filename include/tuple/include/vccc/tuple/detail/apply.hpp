# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TUPLE_DETAIL_APPLY_HPP
# define VCCC_TUPLE_DETAIL_APPLY_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/functional.hpp"

namespace vccc {
namespace detail {

template<class F, class Tuple, std::size_t... I>
constexpr inline decltype(auto)
apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
  return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}

} // namespace detail
} // namespace vccc

#endif // VCCC_TUPLE_DETAIL_APPLY_HPP
