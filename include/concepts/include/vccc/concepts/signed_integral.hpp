//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_
#define VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

/// @addtogroup concepts
/// @{

template<typename T>
struct signed_integral : conjunction<std::is_integral<T>, std::is_signed<T>> {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_
