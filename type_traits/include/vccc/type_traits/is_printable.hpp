//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP
#define VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP

#include "vccc/type_traits/void_t.hpp"
#include <iostream>

namespace vc{
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, void_t<
    decltype(std::cout << std::declval<T>())
>> : std::true_type {};

}

#endif //VCCC_TYPE_TRAITS_IS_PRINTABLE_HPP
