# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_NEGATION_HPP
# define VCCC_TYPE_TRAITS_NEGATION_HPP
#
# include <type_traits>
# 
# include "vccc/type_traits/bool_constant.hpp"

namespace vccc {

/**
 * negation
 * @tparam B
 */
template<typename B> struct negation : bool_constant<!bool(B::value)> {};

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_NEGATION_HPP