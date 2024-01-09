//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_
#define VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_

#include <type_traits>

#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type is an integral type that is signed

```.cpp
template<typename T>
struct signed_integral : conjunction<std::is_integral<T>, std::is_signed<T>> {};
```

The concept `%signed_integral<T>` is satisfied if and only if `T` is an integral type and `std::is_signed<T>::value` is `true`.

@sa [std::signed_integral](https://en.cppreference.com/w/cpp/concepts/signed_integral)
*/
template<typename T>
struct signed_integral : conjunction<std::is_integral<T>, std::is_signed<T>> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SIGNED_INTEGRAL_HPP_
