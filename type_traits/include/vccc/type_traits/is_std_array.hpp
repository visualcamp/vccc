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

template<typename ...>
struct is_std_array : std::false_type {};

template<typename T, std::size_t n>
struct is_std_array<std::array<T, n>> : std::true_type {};
}

# endif //VCCC_TYPE_TRAITS_IS_STD_ARRAY_HPP
