//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_RANGES_VIEWABLE_RANGE_HPP
#define VCCC_RANGES_VIEWABLE_RANGE_HPP

#include <type_traits>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_initializer_list.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = ranges::range<T>::value /* false */>
struct viewable_range_impl : std::false_type {};

template<typename T>
struct viewable_range_impl<T, true>
    : disjunction<
          conjunction<
              ranges::view<remove_cvref_t<T>>,
              constructible_from<remove_cvref_t<T>, T> >,
          conjunction<
              negation< ranges::view<remove_cvref_t<T>> >,
              disjunction<
                  std::is_lvalue_reference<T>,
                  conjunction<
                      movable<std::remove_reference_t<T>>,
                      negation< is_initializer_list<remove_cvref_t<T>> >
                  >
              >
          >
      >{};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief specifies the requirements for a \ref ranges::range "range" to be safely convertible to a \ref ranges::view "view"

The `%viewable_range` concept is a refinement of \ref ranges::range "range" that describes a range that can be converted
into a \ref ranges::view "view" through `views::all`.

@sa [std::ranges::viewable_range](https://en.cppreference.com/w/cpp/ranges/viewable_range)
 */
template<typename T>
struct viewable_range : detail::viewable_range_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWABLE_RANGE_HPP
