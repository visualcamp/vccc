//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/tuple.hpp"
#include "test_core.hpp"

namespace vccc {
namespace {

int Test() {
  INIT_TEST("vccc::tuple")

  static_assert(tuple_like<int>::value == false, " ");
  static_assert(tuple_like<void>::value == false, " ");

  static_assert(tuple_like<std::tuple<>>::value == true, " ");
  static_assert(tuple_like<std::tuple<int>>::value == true, " ");
  static_assert(tuple_like<std::tuple<int, float, double>>::value == true, " ");
  static_assert(tuple_like<std::tuple<int, float, double>&>::value == true, " ");
  static_assert(tuple_like<std::tuple<int, float, double>&&>::value == true, " ");
  static_assert(tuple_like<std::pair<int, float>>::value == true, " ");
  static_assert(tuple_like<std::array<int, 0>>::value == true, " ");
  static_assert(tuple_like<std::array<int, 1>>::value == true, " ");
  static_assert(tuple_like<std::array<int, 10>>::value == true, " ");

  static_assert(pair_like<int>::value == false, " ");
  static_assert(pair_like<std::pair<int, float>>::value == true, " ");
  static_assert(pair_like<std::tuple<int, float>>::value == true, " ");
  static_assert(pair_like<std::tuple<int, float, float>>::value == false, " ");
  static_assert(pair_like<std::array<int, 2>>::value == true, " ");
  static_assert(pair_like<std::array<int, 3>>::value == false, " ");

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
