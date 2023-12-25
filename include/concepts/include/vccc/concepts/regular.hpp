//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_REGULAR_HPP_
#define VCCC_CONCEPTS_REGULAR_HPP_

#include "vccc/concepts/semiregular.hpp"
#include "vccc/concepts/equality_comparable.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

template<typename T>
struct regular
    : conjunction<
        semiregular<T>,
        equality_comparable<T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_REGULAR_HPP_
