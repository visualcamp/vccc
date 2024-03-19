//
// Created by YongGyu Lee on 2023/12/27.
//

#include "test_core.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "vccc/algorithm.hpp"
#include "vccc/string_view.hpp"
#include "vccc/ranges.hpp"
#include "vccc/utility.hpp"

namespace {

int Test() {
  INIT_TEST("vccc::algorithm")

  namespace ranges = vccc::ranges;
  namespace views = vccc::views;
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

  { // fold_left
    std::cout << "Line " << __LINE__ << ", fold_left:\n";

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    int sum = ranges::fold_left(v.begin(), v.end(), 0, std::plus<>{});
    TEST_ENSURES(sum == 36);

    int mul = ranges::fold_left(v, 1, std::multiplies<>{});
    TEST_ENSURES(mul == 1*2*3*4*5*6*7*8);

    std::vector<std::pair<char, int>> data {{'A', 2}, {'B', 3}, {'C', 4}};
    auto sec = ranges::fold_left(
        data | ranges::views::values, 2, std::multiplies<>()
    );
    TEST_ENSURES(sec == 48);

    std::string str = ranges::fold_left(
        v, "A", [](std::string s, int x) { return s + ':' + std::to_string(x); }
    );
    std::cout << "str: " << str << '\n';
    TEST_ENSURES(str == "A:1:2:3:4:5:6:7:8");
  }

  { // ranges::max_element
    const auto v = {3, 1, -14, 1, 5, 9, -14, 9};

    auto result = ranges::max_element(v.begin(), v.end());
    TEST_ENSURES(ranges::distance(v.begin(), result) == 5);

    auto abs_compare = [](int a, int b) { return std::abs(a) < std::abs(b); };
    TEST_ENSURES(ranges::max_element(v, abs_compare) == ranges::begin(v) + 2);
  }

  { // ranges::max
    TEST_ENSURES(ranges::max({0B10, 0X10, 010, 10}) == 16);
    TEST_ENSURES(ranges::max(1, 9999) == 9999);
    TEST_ENSURES(ranges::max('a', 'b') == 'b');
    TEST_ENSURES(ranges::max({"foo"_sv, "bar"_sv, "hello"_sv}, {}, &vccc::string_view::size) == "hello"_sv);
  }

  { // ranges::search
    std::cout << "Line " << __LINE__ << ", ranges::search:\n";

    constexpr auto haystack {"abcd abcd"_sv};
    constexpr auto needle {"bcd"_sv};

    // the search uses iterator pairs begin()/end():
    constexpr auto found1 = vccc::ranges::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end());
    TEST_ENSURES(found1 && vccc::ranges::distance(haystack.begin(), found1.begin()) == 1);

    constexpr auto found2 = vccc::ranges::search(haystack, needle);
    TEST_ENSURES(found2 && vccc::ranges::distance(haystack.begin(), found1.begin()) == 1);

    constexpr auto none {""_sv};
    constexpr auto found3 = vccc::ranges::search(haystack, none);
    TEST_ENSURES(found3.empty());

    constexpr auto awl {"efg"_sv};
    constexpr auto found4 = vccc::ranges::search(haystack, awl);
    TEST_ENSURES(found4.empty());

    constexpr auto bodkin {"234"_sv};
    auto found5 = vccc::ranges::search(haystack, bodkin,
        [](const int x, const int y) { return x == y; }, // pred
        [](const int x) { return std::toupper(x); }, // proj1
        [](const int y) { return y + 'A' - '1'; }); // proj2
    TEST_ENSURES(found5 && vccc::ranges::distance(haystack.begin(), found5.begin()) == 1);
  }

  { // ranges::lexicographical_compare
    std::cout << "Line " << __LINE__ << ", ranges::lexicographical_compare:\n";

    std::vector<char> v1 {'a', 'b', 'c', 'd'};
    std::vector<char> v2 {'a', 'b', 'c', 'd'};

    TEST_ENSURES(ranges::lexicographical_compare(v1, v2) == false);
    TEST_ENSURES(ranges::lexicographical_compare("dabc"_sv, "cbda"_sv) == false);
    TEST_ENSURES(ranges::lexicographical_compare("bdac"_sv, "adcb"_sv) == false);
    TEST_ENSURES(ranges::lexicographical_compare("acdb"_sv, "cdab"_sv) == true);
  }

  { // ranges::set_intersection
    const auto in1 = {1, 2, 2, 3, 4, 5, 6};
    const auto in2 = {2, 2, 3, 3, 5, 7};
    std::vector<int> out {};

    ranges::set_intersection(in1, in2, std::back_inserter(out));

    TEST_ENSURES(ranges::equal(out, {2, 2, 3, 5}));

    std::vector<int> v1{7, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> v2{5, 7, 9, 7};
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<int> v_intersection;
    ranges::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                             std::back_inserter(v_intersection));
    TEST_ENSURES(ranges::equal(v_intersection, {5, 7, 7}));
  }

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
