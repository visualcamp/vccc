# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP
# define VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP
#
# include <ostream>
# include <type_traits>
#
# include "vccc/__type_traits/void_t.hpp"

namespace vccc {

/**
@addtogroup type_traits
@{
    @defgroup is_printable is_printable
    @brief check if a type is default printable
@}

@addtogroup is_printable
@{
*/
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, void_t<
    decltype(std::declval<std::ostream&>() << std::declval<const T&>())
>> : std::true_type {};

//! @} is_printable

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP
