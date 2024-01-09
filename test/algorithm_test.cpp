//
// Created by YongGyu Lee on 2023/12/27.
//

#include "test_core.hpp"

#include <vector>

#include "vccc/algorithm.hpp"
#include "vccc/string_view.hpp"

namespace {

int Test() {
  INIT_TEST("vccc::algorithm")

  namespace ranges = vccc::ranges;
  using namespace vccc::string_view_literals;

  TEST_ENSURES((ranges::min(1, 9999) == 1));
  TEST_ENSURES((ranges::min('a', 'b') == 'a'));
  TEST_ENSURES(ranges::min({"foo"_sv, "bar"_sv, "hello"_sv}, {}, &vccc::string_view::size) == "foo"_sv);

  std::vector<int> v = {1, 2, 3, 4};
  TEST_ENSURES(ranges::min_element(v) == v.begin());
  TEST_ENSURES(ranges::min(v) == 1);

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
