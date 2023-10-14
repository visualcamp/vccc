# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ITERABLE_HPP
# define VCCC_TYPE_TRAITS_ITERABLE_HPP
#
# include "vccc/type_traits/conjunction.hpp"

namespace vccc {

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


/** iterable */

template<typename T>
struct is_iterable : disjunction<is_iterator<T>, std::is_pointer<T>> {};

//! @} is_iterator

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_ITERABLE_HPP
