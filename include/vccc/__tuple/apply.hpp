# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TUPLE_APPLY_HPP
# define VCCC_TUPLE_APPLY_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/__tuple/detail/apply.hpp"

namespace vccc {

//! @addtogroup tuple
//! @{

/**
 * @brief calls a function with the elements of tuple of arguments
 *
 * Invoke the Callable object `f` with the elements of `t` as arguments.
 *
 * @param f Callable object to be invoked
 * @param t tuple whose elements to be used as arguments to `f`
 * @return The value returned by `f`.
 *
 * @par Note
 * Tuple need not be `std::tuple`, and instead may be anything that supports
 * `std::get` and `std::tuple_size`; in particular, `std::array` and `std::pair` may be used.
 *
 * @par Example
@code{.cpp}
#include <iostream>
#include <tuple>
#include <utility>

#include "vccc/tuple.hpp"

int add(int first, int second) { return first + second; }

template<typename T>
T add_generic(T first, T second) { return first + second; }

auto add_lambda = [](auto first, auto second) { return first + second; };

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple)
{
    std::apply
    (
        [&os](Ts const&... tupleArgs)
        {
            os << '[';
            std::size_t n{0};
            ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
}

int main()
{
    // OK
    std::cout << vccc::apply(add, std::pair(1, 2)) << '\n';

    // Error: can't deduce the function type
    // std::cout << vccc::apply(add_generic, std::make_pair(2.0f, 3.0f)) << '\n';

    // OK
    std::cout << vccc::apply(add_lambda, std::pair(2.0f, 3.0f)) << '\n';

    // advanced example
    std::tuple myTuple{25, "Hello", 9.31f, 'c'};
    std::cout << myTuple << '\n';
}
@endcode

Output:
@code
3
5
[25, Hello, 9.31, c]
@endcode
 */
template<class F, class Tuple>
constexpr inline
decltype(auto)
apply(F&& f, Tuple&& t) {
  return internal::apply_impl(
      std::forward<F>(f), std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

//! @}

} // namespace vccc

# endif // VCCC_TUPLE_APPLY_HPP
