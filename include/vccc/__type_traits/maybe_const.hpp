//
// Created by YongGyu Lee on 1/26/24.
//

#ifndef VCCC_TYPE_TRAITS_MAYBE_CONST_HPP
#define VCCC_TYPE_TRAITS_MAYBE_CONST_HPP

#include <type_traits>

namespace vccc {

template<bool Const, typename V>
using maybe_const = std::conditional_t<Const, const V, V>;

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_MAYBE_CONST_HPP
