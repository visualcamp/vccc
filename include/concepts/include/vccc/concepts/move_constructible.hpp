//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_

#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be move constructed

```.cpp
template<typename T>
struct move_constructible : conjunction<constructible_from<T, T>, convertible_to<T, T>> {};
```

The concept move_constructible is satisfied if `T` is a reference type, or if it is an object type where an object of
that type can be constructed from an rvalue of that type in both direct- and copy-initialization contexts, with the
usual semantics.

@sa [std::move_constructible](https://en.cppreference.com/w/cpp/concepts/move_constructible)
 */
template<typename T>
struct move_constructible : conjunction<constructible_from<T, T>, convertible_to<T, T>> {};

/// @}


} // namespace vccc

#endif // VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_
