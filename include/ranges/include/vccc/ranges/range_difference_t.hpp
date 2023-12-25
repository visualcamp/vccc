//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_
#define VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_

#include <type_traits>

#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {

template<
    typename R,
    bool =
        conjunction<
          has_typename_type<iter_difference<R>>,
          has_typename_type<ranges::iterator<R>>
        >::value
>
struct range_difference {};

template<typename R>
struct range_difference<R, true> {
  using type = typename iter_difference_t<ranges::iterator_t<R>>;
};


/// @addtogroup ranges
/// @{


/**
@brief Used to obtain the difference type of the iterator type of range type `R`.
*/

template<typename R>
using range_difference_t = typename range_difference<R>::type;

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_
