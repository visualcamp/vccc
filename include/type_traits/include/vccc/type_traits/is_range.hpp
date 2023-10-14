# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_RANGE_HPP
# define VCCC_TYPE_TRAITS_IS_RANGE_HPP
#
# include "vccc/type_traits/void_t.hpp"

namespace vccc {

/** @addtogroup type_traits
@{
    @defgroup is_range is_range
    @brief check if a type is range
@}
@addtogroup is_range
@{ */

/**
@brief check if a type met range requirements
 */
template<typename T, typename = void>
struct is_range : std::false_type {};

template<typename T>
struct is_range<T, void_t<decltype(std::declval<T>().begin()),
                          decltype(std::declval<T>().end())
>> : std::true_type {
};

template<typename T>
using is_range_t = typename is_range<T>::type;

//! @} is_range

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_IS_RANGE_HPP
