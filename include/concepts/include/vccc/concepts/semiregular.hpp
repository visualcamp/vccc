//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_SEMIREGULAR_HPP_
#define VCCC_CONCEPTS_SEMIREGULAR_HPP_

#include "vccc/concepts/copyable.hpp"
#include "vccc/concepts/default_initializable.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be copied, moved, swapped, and default constructed

The `%semiregular% concept specifies that a type is both copyable and default constructible. It is satisfied by types
that behave similarly to built-in types like `int`, except that they need not support comparison with `==`.

@sa [std::semiregular](https://en.cppreference.com/w/cpp/concepts/semiregular)
@sa regular
 */
template<typename T>
struct semiregular
    : conjunction<
        copyable<T>,
        default_initializable<T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SEMIREGULAR_HPP_
