//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_UNSIGNED_INTEGRAL_HPP
#define VCCC_CONCEPTS_UNSIGNED_INTEGRAL_HPP

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/negation.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type is an integral type that is unsigned

```.cpp
template<typename T>
struct unsigned_integral : conjunction<std::is_integral<T>, negation<std::is_signed<T>>> {};
```

The concept `%unsigned_integral<T>` is satisfied if and only if `T` is an integral type and
`std::is_signed<T>::value` is `false`.

@sa [std::unsigned_integral](https://en.cppreference.com/w/cpp/concepts/unsigned_integral)
*/
template<typename T>
struct unsigned_integral : conjunction<std::is_integral<T>, negation<std::is_signed<T>>> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_UNSIGNED_INTEGRAL_HPP
