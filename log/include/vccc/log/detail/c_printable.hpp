# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_LOG_DETAIL_C_PRINTABLE_HPP
# define VCCC_LOG_DETAIL_C_PRINTABLE_HPP

#include "vccc/type_traits.hpp"

namespace vccc{ namespace detail{

template<typename ...>
struct are_types_c_printable : std::false_type {};

template<typename T>
struct are_types_c_printable<T> : std::false_type {};

template<typename T, typename ...Ts>
struct are_types_c_printable<T, Ts...>
    : std::integral_constant<bool,
        (std::is_same<char *, std::decay_t<T>>::value || std::is_same<const char*, std::decay_t<T>>::value) &&
        are_scalar_v<Ts...>> {};

}}

# endif //VCCC_LOG_DETAIL_C_PRINTABLE_HPP
