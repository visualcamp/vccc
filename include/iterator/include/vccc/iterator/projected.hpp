//
// Created by cosge on 2023-12-29.
//

#ifndef VCCC_ITERATOR_PROJECTED_HPP
#define VCCC_ITERATOR_PROJECTED_HPP

#include "vccc/iterator/indirect_result_t.hpp"
#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/indirectly_regular_unary_invocable.hpp"
#include "vccc/iterator/weakly_incrementable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_complete.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

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
           >::value
>
struct projected_impl_2 {
  struct type : projected_impl_difference_type<I> {
    using value_type = remove_cvref_t<indirect_result_t<Proj, I>>;

    indirect_result_t<Proj&, I> operator*() const;
  };
};

template<typename I, typename Proj>
struct projected_impl_2<I, Proj, false> {};

template<
    typename I,
    typename Proj,
    bool = conjunction<
               is_complete<I>,
               is_complete<Proj>
           >::value /* true */
>
struct projected_impl : projected_impl_2<I, Proj> {};

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

<H1>Notes</H1>
`vccc::projected` meets the requirements of C++ 26

@sa [std::projected](https://en.cppreference.com/w/cpp/iterator/projected)

@{
 */

template<typename I, typename Proj>
using projected = typename detail::projected_impl_2<I, Proj>::type;

/// @}
/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_PROJECTED_HPP
