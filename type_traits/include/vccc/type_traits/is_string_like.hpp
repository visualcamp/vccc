# /*
#  * Created by YongGyu Lee on 2021/01/22.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_STRING_LIKE_HPP
# define VCCC_TYPE_TRAITS_IS_STRING_LIKE_HPP
#
# include <type_traits>
# include <string>
# if __cplusplus > 201402L
# include <string_view>
# endif
# include "vccc/type_traits/is_specialization.hpp"
# include "vccc/type_traits/require.hpp"

namespace vccc{

/**
@addtogroup type_traits
@{
    @defgroup is_string_like is_string_like
    @brief check if a type is string-like type
@}

@addtogroup is_string_like
@{
*/

template<typename T>
struct is_string_like
    : public std::integral_constant<bool, is_specialization<std::decay_t<T>, std::basic_string>::value
# if __cplusplus > 201402L
                                       || is_specialization<std::decay_t<T>, std::basic_string_view>::value
# endif
                                          > {};

template<>
struct is_string_like<const char*> : public std::true_type {};

template<typename T>
using is_string_like_t = typename is_string_like<T>::type;

template<typename T>
constexpr bool is_string_like_v = is_string_like<T>::value;

//! @}

}

# endif //VCCC_TYPE_TRAITS_IS_STRING_LIKE_HPP
