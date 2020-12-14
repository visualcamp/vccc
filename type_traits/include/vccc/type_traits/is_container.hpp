# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_CONTAINER_HPP
# define VCCC_TYPE_TRAITS_IS_CONTAINER_HPP
#
# include "vccc/type_traits/void_t.hpp"

namespace vccc {

template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T, void_t<decltype(std::declval<T>().begin()),
                    decltype(std::declval<T>().end()),
                    decltype(std::declval<T>().size())
>> : std::true_type {
};

template<typename T>
using is_container_t = typename is_container<T>::type;

template<typename T>
constexpr auto is_container_v = is_container<T>::value;

}

# endif //VCCC_TYPE_TRAITS_IS_CONTAINER_HPP
