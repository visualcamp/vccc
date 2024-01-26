//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ITERATOR_SORTABLE_HPP
#define VCCC_ITERATOR_SORTABLE_HPP

#include <type_traits>

#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/permutable.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename I1, typename I2, typename Out, typename Comp, typename Proj1, typename Proj2,
    bool = conjunction<input_iterator<I1>, input_iterator<I2>>::value /* false */>
struct sortable_impl : std::false_type {};
template<typename I1, typename I2, typename Out, typename Comp, typename Proj1, typename Proj2>
struct sortable_impl<I1, I2, Out, Comp, Proj1, Proj2, true>
    : conjunction<
          weakly_incrementable<Out>,
          indirectly_copyable<I1, Out>,
          indirectly_copyable<I2, Out>,
          indirect_strict_weak_order<Comp, projected<I1, Proj1>, projected<I2, Proj2>>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I, typename Comp = ranges::less, typename Proj = identity>
struct sortable
    : conjunction<
          permutable<I>,
          indirect_strict_weak_order<Comp, projected<I, Proj>>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_SORTABLE_HPP
