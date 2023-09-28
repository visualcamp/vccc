//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#include "vccc/type_traits.hpp"
#include "vccc/test.hpp"


int main() {
  INIT_TEST("vccc::type_traits")

  { // is_bounded_array
    TEST_ENSURES(vccc::is_bounded_array<int>{} == false);
    TEST_ENSURES(vccc::is_bounded_array<int[]>{} == false);
    TEST_ENSURES(vccc::is_bounded_array<int[3]>{} == true);
    TEST_ENSURES(vccc::is_bounded_array<int[][3]>{} == false);
  }

  { // is_unbounded_array
    TEST_ENSURES(vccc::is_unbounded_array<int>{} == false);
    TEST_ENSURES(vccc::is_unbounded_array<int[]>{} == true);
    TEST_ENSURES(vccc::is_unbounded_array<int[3]>{} == false);
    TEST_ENSURES(vccc::is_unbounded_array<int[][3]>{} == true);
  }

  { // is_swappable
    struct no_copy {
      no_copy() = default;
      no_copy(const no_copy&) = delete;
      no_copy& operator=(const no_copy&) = delete;
      no_copy(no_copy&&) = default;
      no_copy& operator=(no_copy&&) = default;
    };
    struct no_move {
      no_move() = default;
      no_move(const no_move&) = delete;
      no_move& operator=(const no_move&) = delete;
      no_move(no_move&&) = delete;
      no_move& operator=(no_move&&) = delete;
    };
    struct my_swap {
      void swap(my_swap& rhs) {}
    };

    TEST_ENSURES(vccc::is_swappable<int>{} == true);
    TEST_ENSURES(vccc::is_swappable<std::string>{} == true);
    TEST_ENSURES(vccc::is_swappable<void>{} == false);
    TEST_ENSURES(vccc::is_swappable<no_copy>{} == true);
#if !defined(_MSC_VER) || (__cplusplus >= 201703L)
    TEST_ENSURES(vccc::is_swappable<no_move>{} == false);
#endif
    TEST_ENSURES(vccc::is_swappable<my_swap>{} == true);
  }

  return TEST_RETURN_RESULT;
}
