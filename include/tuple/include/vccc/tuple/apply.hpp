# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TUPLE_APPLY_HPP
# define VCCC_TUPLE_APPLY_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/tuple/detail/apply.hpp"

namespace vccc {

//! @addtogroup tuple
//! @{

/** @brief invokes the callable object f with a tuple of arguments
 * @param f callable objects to be invoked
 * @param t tuple whose elements to be used as arguments to f
 * @return the value returned by f
 */

template<class F, class Tuple>
constexpr inline
decltype(auto)
apply(F&& f, Tuple&& t) {
  return internal::apply_impl(
      std::forward<F>(f), std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

//! @}

} // namespace vccc

# endif // VCCC_TUPLE_APPLY_HPP
