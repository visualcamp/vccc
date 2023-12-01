//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/string_view.hpp"
#include "vccc/test.hpp"

namespace vccc {
namespace {

int Test() {
  INIT_TEST("vccc::string_view")

  static_assert(std::is_trivially_copyable<string_view>::value, " ");

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
