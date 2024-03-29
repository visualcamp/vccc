# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_LOG_DETAIL_C_PRINTABLE_HPP
# define VCCC_LOG_DETAIL_C_PRINTABLE_HPP
#
# include <type_traits>
#
# include "vccc/type_traits.hpp"

namespace vccc {
namespace internal {

template<typename T>
struct can_decay_to_scalar_impl : std::conditional<std::is_scalar<T>::value || std::is_array<T>::value,
                                                   std::true_type, std::false_type>::type {};

template<typename ...Ts>
struct can_decay_to_scalar : conjunction<can_decay_to_scalar_impl<Ts>...> {};

template<typename ...Ts>
using can_decay_to_scalar_t = typename can_decay_to_scalar<Ts...>::type;

template<typename ...>
struct are_types_c_printable : std::false_type {};

// block single type to avoid std::snprintf security warnings
template<typename T>
struct are_types_c_printable<T> : std::false_type {};

template<typename T, typename ...Ts>
struct are_types_c_printable<T, Ts...>
    : std::integral_constant<bool,
        (std::is_same<char *, std::decay_t<T>>::value || std::is_same<const char*, std::decay_t<T>>::value) &&
        can_decay_to_scalar<Ts...>::value> {};

template<typename ...Ts>
using are_types_c_printable_t = typename are_types_c_printable<Ts...>::type;

} // namespace internal
} // namespace vccc

# endif // VCCC_LOG_DETAIL_C_PRINTABLE_HPP
