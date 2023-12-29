//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_COMPARABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_COMPARABLE_HPP

#include <type_traits>

#include "vccc/functional/identity.hpp"
#include "vccc/iterator/indirect_binary_predicate.hpp"
#include "vccc/iterator/projected.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename I1,
    typename I2,
    typename Comp,
    typename Proj1,
    typename Proj2,
    bool = conjunction<
               has_typename_type< projected_impl<I1, Proj1> >,
               has_typename_type< projected_impl<I2, Proj2> >
           >::value /* true */
>
struct indirectly_comparable_impl
    : indirect_binary_predicate<Comp, projected<I1, Proj1>, projected<I2, Proj2>> {};

template<typename I1, typename I2, typename Comp, typename Proj1, typename Proj2>
struct indirectly_comparable_impl<I1, I2, Comp, Proj1, Proj2, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that the values referenced by two `indirectly_readable` types can be compared

The concept `%indirectly_comparable` specifies the fundamental algorithm requirement for comparing values across two
independent ranges.

@sa [std::indirectly_comparable](https://en.cppreference.com/w/cpp/iterator/indirectly_comparable)
@sa indirect_binary_predicate
 */
template<typename I1, typename I2, typename Comp,
         typename Proj1 = identity, typename Proj2 = identity>
struct indirectly_comparable : detail::indirectly_comparable_impl<I1, I2, Comp, Proj1, Proj2> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_COMPARABLE_HPP
