//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_
#define VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_

#include "vccc/__concepts/destructible.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a variable of the type can be constructed from or bound to a set of argument types

```
template<typename T, typename... Args>
struct constructible_from : conjunction<
    destructible<T>,
    std::is_constructible<T, Args...>> {};
```

See [`std::constructible_from`](https://en.cppreference.com/w/cpp/concepts/constructible_from)
*/
template<typename T, typename... Args>
struct constructible_from : conjunction<
    destructible<T>,
    std::is_constructible<T, Args...>> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_
