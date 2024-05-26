//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <vccc/functional.hpp>
#include <cmath>

int foo(int a, int b, int c) {
  return (a + b) * c;
}

constexpr int minus(int a, int b) {
  return a - b;
}

struct S {
  int val;
  constexpr int minus(int arg) const noexcept { return val - arg; }
};

int main() {
  INIT_TEST("vccc::functional")

  // normal function
  TEST_ENSURES(vccc::invoke(foo, 3, 4, 5) == 35);

  // function pointer
  auto fp = &foo;
  TEST_ENSURES(vccc::invoke(fp, 3, 4, 5) == 35);

  // template functions
  TEST_ENSURES(vccc::invoke(std::pow<int, int>, 2, 3) == 8);

  // generic lambda
  TEST_ENSURES(vccc::invoke([](auto... args) {
    int sum = 0;
    int dummy[] = {(sum += args, 0)...,};
    return sum;
  }, 1, 2, 3) == 6);

  // lambda with captures
  int x = 10;
  TEST_ENSURES(vccc::invoke([&](auto... args) -> int{
    int sum = x;
    int dummy[] = {(sum += args, 0)..., };
    return sum;
  }, 1, 2, 3) == 16);

  { // bind_front
    auto fifty_minus = vccc::bind_front(minus, 50);
    TEST_ENSURES(fifty_minus(3) == 47); // equivalent to: minus(50, 3) == 47

    auto member_minus = vccc::bind_front(&S::minus, S{50});
    TEST_ENSURES(member_minus(3) == 47); //: S tmp{50}; tmp.minus(3) == 47

    static_assert(noexcept(S{}.minus(3)) == true, "");

    // Noexcept-specification is preserved:
    static_assert(!noexcept(fifty_minus(3)), "");
    static_assert(noexcept(member_minus(3)) == (__cplusplus > 201403L), "");

    // Binding of a lambda:
    auto plus = [](int a, int b) { return a + b; };
    auto forty_plus = vccc::bind_front(plus, 40);
    TEST_ENSURES(forty_plus(7) == 47); // equivalent to: plus(40, 7) == 47

    auto concat = [](auto&& a, auto&& b) { return std::forward<decltype(a)>(a) + std::forward<decltype(b)>(b); };
    auto hello = vccc::bind_front(concat, std::string("hello"));
    TEST_ENSURES(std::move(hello)(", world") == "hello, world");
    TEST_ENSURES(hello(", world") == ", world");
  }

  { // bind_back
    auto madd = [](int a, int b, int c) { return a * b + c; };
    auto mul_plus_seven = vccc::bind_back(madd, 7);
    TEST_ENSURES(mul_plus_seven(4, 10) == 47); //: madd(4, 10, 7) == 47
  }

  return TEST_RETURN_RESULT;
}
