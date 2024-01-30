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

template<typename I, typename Comp, typename Proj,
    bool = conjunction<
        permutable<I>,
        projectable<I, Proj>
    >::value /* false */>
struct sortable_impl : std::false_type {};

template<typename I, typename Comp, typename Proj>
struct sortable_impl<I, Comp, Proj, true>
    : indirect_strict_weak_order<Comp, projected<I, Proj>> {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I, typename Comp = ranges::less, typename Proj = identity>
struct sortable : detail::sortable_impl<I, Comp, Proj> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_SORTABLE_HPP
