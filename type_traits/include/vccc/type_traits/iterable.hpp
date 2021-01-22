# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ITERABLE_HPP
# define VCCC_TYPE_TRAITS_ITERABLE_HPP
#
# include "vccc/type_traits/are.hpp"

namespace vccc{


/**
@addtogroup type_traits
@{
    @defgroup is_iterator is_iterator
    @brief check if a type is a iterable type

    std::iterator or pointer type
@}

@addtogroup is_iterator
@{
*/
template<typename T, typename = void>
struct is_iterator : std::false_type {};

template<typename T>
struct is_iterator<T,
                   typename std::enable_if_t<
                       !std::is_same<
                           typename std::iterator_traits<T>::value_type,
                           void
                       >::value
                   >
> : std::true_type {
};

template<typename T>
using is_iterator_t = typename is_iterator<T>::type;

template<typename T>
constexpr auto is_iterator_v = is_iterator<T>::value;


/** iterable */

template<typename ...Ts>
constexpr auto iterable = are_v<is_iterator<Ts>...> || are_v<std::is_pointer<Ts>...>;

//! @} is_iterator

}

# endif //VCCC_TYPE_TRAITS_ITERABLE_HPP
