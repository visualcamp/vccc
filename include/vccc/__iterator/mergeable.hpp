//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ITERATOR_MERGEABLE_HPP
#define VCCC_ITERATOR_MERGEABLE_HPP

#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/indirectly_copyable.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename I1, typename I2, typename Out, typename Comp, typename Proj1, typename Proj2,
    bool = conjunction<input_iterator<I1>, input_iterator<I2>>::value /* false */>
struct mergeable_impl_1 : std::false_type {};
template<typename I1, typename I2, typename Out, typename Comp, typename Proj1, typename Proj2>
struct mergeable_impl_1<I1, I2, Out, Comp, Proj1, Proj2, true>
    : conjunction<
          weakly_incrementable<Out>,
          indirectly_copyable<I1, Out>,
          indirectly_copyable<I2, Out>,
          indirect_strict_weak_order<Comp, projected<I1, Proj1>, projected<I2, Proj2>>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I1, typename I2, typename Out,
    typename Comp = ranges::less, typename Proj1 = identity, typename Proj2 = identity>
struct mergeable : detail::mergeable_impl_1<I1, I2, Out, Comp, Proj1, Proj2> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_MERGEABLE_HPP
