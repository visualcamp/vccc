//
// Created by yonggyulee on 1/12/24.
//

#ifndef VCCC_ALGORITHM_COPY_HPP
#define VCCC_ALGORITHM_COPY_HPP

#include <type_traits>
#include <utility>

#include "vccc/__algorithm/in_out_result.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__iterator/indirectly_copyable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {

template<typename I, typename O> using copy_result = in_out_result<I, O>;
template<typename I, typename O> using copy_if_result = in_out_result<I, O>;

namespace detail {

struct copy_niebloid {
 private:
  template<typename R, typename O, bool = input_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename O>
  struct check_range<R, O, true>
      : conjunction<
          weakly_incrementable<O>,
          indirectly_copyable<iterator_t<R>, O>,
          has_typename_type<borrowed_iterator<R>>
        > {};

 public:
  template<typename I, typename S, typename O, std::enable_if_t<conjunction<
    input_iterator<I>,
    sentinel_for<S, I>,
    weakly_incrementable<O>,
    indirectly_copyable<I, O>
  >::value, int> = 0>
  constexpr copy_result<I, O> operator()(I first, S last, O result) const {
    for (; first != last; ++first, (void)++result)
      *result = *first;
    return {std::move(first), std::move(result)};
  }

  template<typename R, typename O, std::enable_if_t<check_range<R, O>::value, int> = 0>
  constexpr copy_result<borrowed_iterator_t<R>, O> operator()(R&& r, O result) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::move(result));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::copy_niebloid copy{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_COPY_HPP
