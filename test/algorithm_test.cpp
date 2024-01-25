//
// Created by YongGyu Lee on 2023/12/27.
//

#include "test_core.hpp"

#include <string>
#include <vector>

#include "vccc/algorithm.hpp"
#include "vccc/string_view.hpp"
#include "vccc/utility.hpp"

namespace {

int Test() {
  INIT_TEST("vccc::algorithm")

  namespace ranges = vccc::ranges;
  using namespace vccc::string_view_literals;
  {
    TEST_ENSURES((ranges::min(1, 9999) == 1));
    TEST_ENSURES((ranges::min('a', 'b') == 'a'));
    TEST_ENSURES(ranges::min({"foo"_sv, "bar"_sv, "hello"_sv}, {}, &vccc::string_view::size) == "foo"_sv);

    std::vector<int> v = {1, 2, 3, 4};
    TEST_ENSURES(ranges::min_element(v) == v.begin());
    TEST_ENSURES(ranges::min(v) == 1);
  }

  { // ranges::for_each
    std::vector<int> nums = {1, 2, 3, 4, 5};
    auto print = [](const auto& x) { std::cout << ' ' << x; };

    ranges::for_each(vccc::as_const(nums), print);
    print('\n');
    struct Sum {
      void operator()(int n) { sum += n; }
      int sum {0};
    };

    ranges::for_each(nums, [](auto& x) { ++x; });
    TEST_ENSURES(nums.front() == 2);
    TEST_ENSURES(nums.back() == 6);

    auto result = ranges::for_each(nums.begin(), nums.end(), Sum{});
    TEST_ENSURES(result.in == nums.end());
    TEST_ENSURES(result.fun.sum == 20);

    ranges::for_each(vccc::as_const(nums), print);
    print('\n');

    using pair = vccc::compressed_pair<int, std::string>;
    std::vector<pair> pairs = {{1, "one"}, {2, "two"}, {3, "three"}};

    std::cout << "project the pair::first";
    ranges::for_each(pairs, print, [](const pair& p) { return p.first(); });
    print('\n');

    using std_pair = std::pair<int, std::string>;
    std::vector<std_pair> pairs2 = {{1, "one"}, {2, "two"}, {3, "three"}};
    std::cout << "project the pair::second";
    ranges::for_each(pairs2, print, &std_pair::second);
    print('\n');
  }

  {
    std::vector<std::string> v = {"a", "b", "c", "d", "e", "f", "g"};
    const std::vector<std::string> v1 = {"d", "e", "f", "g", "", "", ""};
    const std::vector<std::string> v2 = {"", "", "d", "e", "f", "g", ""};

    vccc::shift_left(begin(v), end(v), 3);
    TEST_ENSURES((vccc::ranges::equal(v, v1)));

    vccc::shift_right(begin(v), end(v), 2);
    TEST_ENSURES((vccc::ranges::equal(v, v2)));

    vccc::shift_left(begin(v), end(v), 8);
    TEST_ENSURES((vccc::ranges::equal(v, v2)));
  }

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
