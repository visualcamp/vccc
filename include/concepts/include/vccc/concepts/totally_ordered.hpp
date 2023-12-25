//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_
#define VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_

#include <type_traits>

#include "vccc/concepts/equality_comparable.hpp"
#include "vccc/concepts/partially_ordered_with.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{


/**
 *  @brief specifies that the comparison operators on the type yield a total order
 *
 *  The concept `totally_ordered` specifies that the comparison operators `==`,`!=`,`<`,`>`,`<=`,`>=` on a type yield
 *  results consistent with a [strict total order](https://en.wikipedia.org/wiki/Total_order#Strict_and_non-strict_total_orders)
 *  on the type.
 */
template<typename T>
struct totally_ordered
    : conjunction<
        equality_comparable<T>,
        partially_ordered_with<T, T>
      >{};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_
