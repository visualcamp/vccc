//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <array>

#include "vccc/concepts.hpp"

namespace vccc {
namespace {

template<int v>
using subsume_value = std::integral_constant<int, v>;

template<typename...>
struct subsume : subsume_value<0> {};

template<typename...T>
struct subsume<int, T...> : subsume_value<1> {};

template<typename...T>
struct subsume<int, int, T...> : subsume_value<2> {};

template<int v>
using subsume_value = std::integral_constant<int, v>;

template< class T, class U>
std::enable_if_t<concepts::same_as<U, T>::value>
foo(T a, U b) {
  std::cout << "Not integral" << std::endl;
}

template< class T, class U>
std::enable_if_t<conjunction<concepts::same_as<T, U>, std::is_integral<T>>::value>
foo(T a, U b) {
  std::cout << "Integral" << std::endl;
}

int Test() {
  INIT_TEST("vccc::concepts")

  { // convertible_to
    struct From;
    struct To {
      explicit To(From) = delete;
    };
    struct From {
      operator To();
    };

    static_assert(std::is_convertible<From, To>::value, " ");
    static_assert(not concepts::convertible_to<From, To>::value, " ");
  }

  {
    foo(1, 2);
  }


  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
