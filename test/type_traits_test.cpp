//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#include "vccc/type_traits.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "test_core.hpp"

template<typename T1, typename T2, typename = void>
struct ter_impl {};

template<typename T1, typename T2>
struct ter_impl<T1, T2, vccc::void_t<decltype(false ? std::declval<T1>() : std::declval<T2>())>> {
  using type = decltype(false ? std::declval<T1>() : std::declval<T2>());
};

template<typename T1, typename T2>
struct ter : ter_impl<T1, T2> {};

template<template<typename> class T>
struct tc {};

template<typename T>
struct outer {
  template<typename U>
  struct inner {};
};

namespace my_swap_namespace {

struct swappable_object {
  swappable_object() = default;
  swappable_object(swappable_object&&) = delete;
  int x = 1;
};

void swap(swappable_object &a, swappable_object &b) {
  a.x = 10;
  b.x = 20;
}

} // namespace my_swap_namespace

int main() {
  INIT_TEST("vccc::type_traits")

  static_assert(std::is_same<std::add_rvalue_reference_t<std::remove_reference_t<int&>>, int&&>::value, " ");

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
    TEST_ENSURES(vccc::is_swappable<no_move>{} == false);
    TEST_ENSURES((vccc::is_swappable<std::pair<no_move, no_move>>{} == false));
    TEST_ENSURES(vccc::is_swappable<my_swap>{} == true);


    TEST_ENSURES(vccc::is_swappable<my_swap_namespace::swappable_object>{} == true);
  }

  {


//    static_assert(std::is_same<decltype(k), int&>::value, " ");
  }

  {
    auto il = {1, 2, 3, 4};
    static_assert(vccc::is_initializer_list<decltype(il)>::value, "");

    static_assert(vccc::is_initializer_list<std::initializer_list<int>>::value, "");
    static_assert(vccc::is_initializer_list<std::initializer_list<int&>>::value, "");
    static_assert(vccc::is_initializer_list<std::initializer_list<int>&>::value, "");
    static_assert(vccc::is_initializer_list<const volatile std::initializer_list<int>&&>::value, "");
  }

  return TEST_RETURN_RESULT;
}
