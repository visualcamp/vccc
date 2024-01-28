//
// Created by cosge on 2023-12-29.
//

#ifndef VCCC_ITERATOR_PROJECTED_HPP
#define VCCC_ITERATOR_PROJECTED_HPP

#include "vccc/__iterator/indirect_result_t.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/indirectly_regular_unary_invocable.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = weakly_incrementable<I>::value /* true */>
struct projected_impl_difference_type {
  using difference_type = iter_difference_t<I>;
};
template<typename I>
struct projected_impl_difference_type<I, false> {};

template<
    typename I,
    typename Proj,
    bool = conjunction<
               indirectly_readable<I>,
               indirectly_regular_unary_invocable<Proj, I>
           >::value /* true */
>
struct projected_impl {
  struct type : projected_impl_difference_type<I> {
    using value_type = remove_cvref_t<indirect_result_t<Proj, I>>;

    indirect_result_t<Proj&, I> operator*() const;
  };
};

template<typename I, typename Proj>
struct projected_impl<I, Proj, false> {};

} // namespace detail

/**
@addtogroup iterator
@{
@addtogroup iterator_projected__class__projected projected
@brief helper template for specifying the constraints on algorithms that accept projections

`%projected` is used only to constrain algorithms that accept callable objects and projections,
and hence its `operator*()` is not defined.

@warning `%vccc::projected` meets the requirements of C++ 26, but without using
[constraints](https://en.cppreference.com/w/cpp/language/constraints) user must check its validity with
`vccc::projectable<I, Proj>`before using `%vccc::projected`

@sa [std::projected](https://en.cppreference.com/w/cpp/iterator/projected)

@{
*/

template<typename I, typename Proj>
using projected = typename detail::projected_impl<I, Proj>::type;

/// @brief Test if expression `vccc::projected<I, Proj>` is valid in an unqualified context
template<typename I, typename Proj>
using projectable = has_typename_type<detail::projected_impl<I, Proj>>;

/// @}
/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_PROJECTED_HPP
