//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_VIEWS_ELEMENTS_HPP
#define VCCC_RANGES_VIEWS_ELEMENTS_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/elements_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<std::size_t N>
class elements_adaptor_object : public range_adaptor_closure<elements_adaptor_object<N>> {
 public:
  constexpr elements_adaptor_object() = default;

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return elements_view<all_t<R>, N>(std::forward<R>(r));
  }
};

} // namespace detail

/**
@addtogroup ranges
@{
@defgroup ranges_elements_view__class__Adaptors ranges::elements_view, views::elements
@brief takes a `view` consisting of `tuple-like` values and a number N and produces a `view` of \f$ { N^{th}} \f$ element of
each tuple

## Example
```.cpp
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "vccc/ranges.hpp"

int main()
{
    const std::vector<std::tuple<int, char, std::string>> vt
    {
        {1, 'A', "α"},
        {2, 'B', "β"},
        {3, 'C', "γ"},
        {4, 'D', "δ"},
        {5, 'E', "ε"},
    };

    for (int const e : vccc::views::elements<0>(vt))
        std::cout << e << ' ';
    std::cout << '\n';

    for (char const e : vt | vccc::views::elements<1>)
        std::cout << e << ' ';
    std::cout << '\n';

    for (std::string const& e : vccc::views::elements<2>(vt))
        std::cout << e << ' ';
    std::cout << '\n';
}
```
#### Output
```
1 2 3 4 5
A B C D E
α β γ δ ε
```

@sa [std::views::elements](https://en.cppreference.com/w/cpp/ranges/elements_view)
@}
*/

/// @addtogroup ranges_elements_view__class__Adaptors
/// @{

template<std::size_t N>
VCCC_INLINE_OR_STATIC constexpr detail::elements_adaptor_object<N> elements{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ELEMENTS_HPP
