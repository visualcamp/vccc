# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_STD_ARRAY_HPP
# define VCCC_TYPE_TRAITS_IS_STD_ARRAY_HPP
#
# include <array>
# include <type_traits>

namespace vccc{

/**
@addtogroup type_traits
@{
    @defgroup is_std_array is_std_array
    @brief check if a type is std::array
@}

@addtogroup is_std_array
@{
*/
template<typename ...>
struct is_std_array : std::false_type {};

template<typename T, std::size_t n>
struct is_std_array<std::array<T, n>> : std::true_type {};

//! @} is_std_array

}

# endif //VCCC_TYPE_TRAITS_IS_STD_ARRAY_HPP
