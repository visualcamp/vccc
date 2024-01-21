//
// Created by cosge on 2023-12-29.
//

#ifndef VCCC_TYPE_TRAITS_IS_INITIALIZER_LIST_HPP
#define VCCC_TYPE_TRAITS_IS_INITIALIZER_LIST_HPP

#include <initializer_list>
#include <type_traits>

#include "vccc/__type_traits/is_specialization.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{

template <class T>
struct is_initializer_list : is_specialization<remove_cvref_t<T>, std::initializer_list> {};

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_INITIALIZER_LIST_HPP
