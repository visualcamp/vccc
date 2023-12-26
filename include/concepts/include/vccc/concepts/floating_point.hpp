//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_FLOATING_POINT_HPP
#define VCCC_CONCEPTS_FLOATING_POINT_HPP

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/negation.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type is a floating-point type

```.cpp
template<typename T>
struct floating_point : std::is_floating_point<T> {};
```

The concept `%floating_point<T>` is satisfied if and only if `T` is a floating-point type.

@sa [std::floating_point](https://en.cppreference.com/w/cpp/concepts/floating_point)
*/
template<typename T>
struct floating_point : std::is_floating_point<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_FLOATING_POINT_HPP
