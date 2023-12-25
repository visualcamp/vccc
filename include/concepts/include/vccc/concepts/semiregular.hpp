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

template<typename T>
struct semiregular
    : conjunction<
        copyable<T>,
        default_initializable<T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SEMIREGULAR_HPP_
