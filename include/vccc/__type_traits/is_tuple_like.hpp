# /*
#  * Created by YongGyu Lee on 2022/02/01.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_TUPLE_LIKE_HPP
# define VCCC_TYPE_TRAITS_IS_TUPLE_LIKE_HPP
# 
# include <tuple>
# include <type_traits>

namespace vccc {

template<typename T, typename = void>
struct is_tuple_like : std::false_type {};

template<typename T>
struct is_tuple_like<T, void_t<decltype(std::tuple_size<T>::value)>> : std::true_type {};

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_IS_TUPLE_LIKE_HPP
