//
// Created by yonggyulee on 2023/12/21.
//

#ifndef VCCC_CONCEPTS_SUBTRACTABLE_HPP_
#define VCCC_CONCEPTS_SUBTRACTABLE_HPP_

#include <type_traits>

#include "vccc/concepts/implicit_expression_check.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {

template<typename T, typename U, typename = void>
struct is_explicitly_subtractable : std::false_type {};

template<typename T, typename U>
struct is_explicitly_subtractable<T, U, void_t<decltype(std::declval<T>() - std::declval<U>())>> : std::true_type {};

template<typename T, bool = is_referencable<T>::value>
struct is_subtractable_impl : std::false_type {};

template<typename T>
struct is_subtractable_impl<T, true> : implicit_expression_check<is_explicitly_subtractable, const T&, const T&> {};


/// @addtogroup concepts
/// @{

template<typename T>
struct subtractable : is_subtractable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SUBTRACTABLE_HPP_
