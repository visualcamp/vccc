//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_REGULAR_HPP_
#define VCCC_CONCEPTS_REGULAR_HPP_

#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type is %regular, that is, it is both `semiregular` and `equality_comparable`

@code{.cpp}
template<typename T>
struct regular
    : conjunction<
        semiregular<T>,
        equality_comparable<T>
      > {};
@endcode

The `%regular` concept specifies that a type is \a regular, that is, it is copyable, default constructible,
and equality comparable. It is satisfied by types that behave similarly to built-in types like `int`,
and that are comparable with `==`.

@sa [std::regular](https://en.cppreference.com/w/cpp/concepts/regular)
@sa semiregular
 */
template<typename T>
struct regular
    : conjunction<
        semiregular<T>,
        equality_comparable<T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_REGULAR_HPP_
