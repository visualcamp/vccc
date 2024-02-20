//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <array>
#include <list>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

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

  {
    auto t = std::make_tuple(1, 3.3f, 10.0);
    auto sum = tuple_fold_left(t, 0.0, std::plus<>{});
    TEST_ENSURES(sum == 0.0 + 1 + 3.3f + 10.0);
  }

  {
    auto t = std::make_tuple(1, 2, 3);
    auto sum = tuple_fold_left(
        tuple_transform(t, [](auto x) { return x * 2; }), 0, std::plus<>{});
    TEST_ENSURES(sum == 12);

    auto t2 = std::make_tuple(std::vector<int>{1, 2, 3}, std::string{"456"}, std::list<double>{7, 8, 9});
    auto size_sum = tuple_fold_left(
        tuple_transform(t2, [](auto&& r) { return r.size(); }), 0, std::plus<>{});
    TEST_ENSURES(size_sum == 9);
  }

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
