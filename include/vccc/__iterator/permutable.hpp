//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ITERATOR_PERMUTABLE_HPP
#define VCCC_ITERATOR_PERMUTABLE_HPP

#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirectly_movable_storable.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup iterator
/// @{

template<typename I>
struct permutable
    : conjunction<
          forward_iterator<I>,
          indirectly_movable_storable<I, I>,
          indirectly_swappable<I, I>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_PERMUTABLE_HPP
