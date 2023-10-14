//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <vccc/functional.hpp>
#include <cmath>

int foo(int a, int b, int c) {
  return (a + b) * c;
}

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


  return TEST_RETURN_RESULT;
}
