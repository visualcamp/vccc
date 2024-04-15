//
// Created by YongGyu Lee on 2023/12/27.
//

#include "test_core.hpp"

#include <algorithm>
#include <cctype>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "vccc/array.hpp"
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

  {
    constexpr static auto v = {1, 2, 3, 1, 2, 3, 1, 2};

    {
      auto i1 = ranges::find_last(v.begin(), v.end(), 3);
      auto i2 = ranges::find_last(v, 3);
      TEST_ENSURES(ranges::distance(v.begin(), i1.begin()) == 5);
      TEST_ENSURES(ranges::distance(v.begin(), i2.begin()) == 5);
    }
    {
      auto i1 = ranges::find_last(v.begin(), v.end(), -3);
      auto i2 = ranges::find_last(v, -3);
      TEST_ENSURES(i1.begin() == v.end());
      TEST_ENSURES(i2.begin() == v.end());
    }

    auto abs = [](int x) { return x < 0 ? -x : x; };

    {
      auto pred = [](int x) { return x == 3; };
      auto i1 = ranges::find_last_if(v.begin(), v.end(), pred, abs);
      auto i2 = ranges::find_last_if(v, pred, abs);
      TEST_ENSURES(ranges::distance(v.begin(), i1.begin()) == 5);
      TEST_ENSURES(ranges::distance(v.begin(), i2.begin()) == 5);
    }
    {
      auto pred = [](int x) { return x == -3; };
      auto i1 = ranges::find_last_if(v.begin(), v.end(), pred, abs);
      auto i2 = ranges::find_last_if(v, pred, abs);
      TEST_ENSURES(i1.begin() == v.end());
      TEST_ENSURES(i2.begin() == v.end());
    }

    {
      auto pred = [](int x) { return x == 1 or x == 2; };
      auto i1 = ranges::find_last_if_not(v.begin(), v.end(), pred, abs);
      auto i2 = ranges::find_last_if_not(v, pred, abs);
      TEST_ENSURES(ranges::distance(v.begin(), i1.begin()) == 5);
      TEST_ENSURES(ranges::distance(v.begin(), i2.begin()) == 5);
    }
    {
      auto pred = [](int x) { return x == 1 or x == 2 or x == 3; };
      auto i1 = ranges::find_last_if_not(v.begin(), v.end(), pred, abs);
      auto i2 = ranges::find_last_if_not(v, pred, abs);
      TEST_ENSURES(i1.begin() == v.end());
      TEST_ENSURES(i2.begin() == v.end());
    }

    using P = std::pair<vccc::string_view, int>;
    std::forward_list<P> list
        {
            {"one", 1}, {"two", 2}, {"three", 3},
            {"one", 4}, {"two", 5}, {"three", 6},
        };
    auto cmp_one = [](const vccc::string_view &s) { return s == "one"; };

    // find latest element that satisfy the comparator, and projecting pair::first
    const auto subrange = ranges::find_last_if(list, cmp_one, &P::first);
    TEST_ENSURES(ranges::equal(subrange, std::vector<P>{{"one", 4}, {"two", 5}, {"three", 6}}));
  }

  { // ranges::find_end
    constexpr auto secret{"password password word..."_sv};
    constexpr auto wanted{"password"_sv};

    auto s1 = ranges::find_end(secret.cbegin(), secret.cend(), wanted.cbegin(), wanted.cend());
    TEST_ENSURES(std::distance(secret.begin(), s1.begin()) == 9 && s1.size() == 8);

    auto s2 = ranges::find_end(secret, "word"_sv);
    TEST_ENSURES(std::distance(secret.begin(), s2.begin()) == 18 && s2.size() == 4);

    auto s3 = ranges::find_end(secret, "ORD"_sv, [](const char x, const char y) {
      return std::tolower(x) == std::tolower(y);
    });
    TEST_ENSURES(std::distance(secret.begin(), s3.begin()) == 19 && s3.size() == 3);

    auto s4 = ranges::find_end(secret, "SWORD"_sv, {}, {}, [](char c) { return std::tolower(c); });
    TEST_ENSURES(std::distance(secret.begin(), s4.begin()) == 12 && s4.size() == 5);
  }

  { // ranges::find_first_of
    constexpr static auto haystack = {1, 2, 3, 4};
    constexpr static auto needles  = {0, 3, 4, 3};

    auto found1 = ranges::find_first_of(haystack.begin(), haystack.end(),
                                               needles.begin(), needles.end());
    TEST_ENSURES(std::distance(haystack.begin(), found1) == 2);

    auto found2 = ranges::find_first_of(haystack, needles);
    TEST_ENSURES(std::distance(haystack.begin(), found2) == 2);

    constexpr static auto negatives = {-6, -3, -4, -3};
    auto not_found = ranges::find_first_of(haystack, negatives);
    TEST_ENSURES(not_found == haystack.end());

    auto found3 = ranges::find_first_of(haystack, negatives,
                                               [](int x, int y) { return x == -y; }); // uses a binary comparator
    TEST_ENSURES(std::distance(haystack.begin(), found3) == 2);

    struct P { int x, y; };
    constexpr static auto p1 = {P{1, -1}, P{2, -2}, P{3, -3}, P{4, -4}};
    constexpr static auto p2 = {P{5, -5}, P{6, -3}, P{7, -5}, P{8, -3}};
    const auto found4 = ranges::find_first_of(p1, p2, {}, &P::y, &P::y);
    TEST_ENSURES(std::distance(p1.begin(), found4) == 2);
  }

  { // ranges::make_heap
    std::vector<int> h{1, 6, 1, 8, 0, 3, 3, 9, 8, 8, 7, 4, 9, 8, 9};

    vccc::ranges::make_heap(h);
    TEST_ENSURES(std::is_heap(h.begin(), h.end()));

    vccc::ranges::make_heap(h, std::greater<>{});
    TEST_ENSURES(std::is_heap(h.begin(), h.end()) == false);
    TEST_ENSURES(std::is_heap(h.begin(), h.end(), std::greater<>{}));
  }

  { // ranges::pop_heap
    auto v = vccc::to_array({3, 1, 4, 1, 5, 9, 2, 6, 5, 3});

    vccc::ranges::make_heap(v);
    for (auto n {vccc::ssize(v)}; n >= 0; --n) {
      vccc::ranges::pop_heap(v.begin(), v.begin() + n);
      TEST_ENSURES(std::is_sorted(v.cbegin() + n, v.cend()));
    }

    vccc::ranges::make_heap(v, vccc::ranges::greater{});
    for (auto n {vccc::ssize(v)}; n >= 0; --n) {
      vccc::ranges::pop_heap(v.begin(), v.begin() + n, vccc::ranges::greater{});
      TEST_ENSURES(std::is_sorted(v.cbegin() + n, v.cend(), vccc::ranges::greater{}));
    }
  }

  { // ranges::sort_heap
    auto v = vccc::to_array({3, 1, 4, 1, 5, 9});
    vccc::ranges::make_heap(v);
    vccc::ranges::sort_heap(v);
    TEST_ENSURES(std::is_sorted(v.begin(), v.end()));

    vccc::ranges::make_heap(v, vccc::ranges::greater{});
    vccc::ranges::sort_heap(v, vccc::ranges::greater{});
    TEST_ENSURES(std::is_sorted(v.begin(), v.end(), vccc::ranges::greater{}));
  }

  { // ranges::sort
    struct Particle {
      std::string name;
      double mass; // MeV
    };

    auto s = vccc::to_array({5, 7, 4, 2, 8, 6, 1, 9, 0, 3});
    namespace ranges = vccc::ranges;

    ranges::sort(s);
    TEST_ENSURES(std::is_sorted(s.begin(), s.end()));

    ranges::sort(s, ranges::greater());
    TEST_ENSURES(std::is_sorted(s.begin(), s.end(), std::greater<>{}));

    Particle particles[] {
        {"Electron", 0.511}, {"Muon", 105.66}, {"Tau", 1776.86},
        {"Positron", 0.511}, {"Proton", 938.27}, {"Neutron", 939.57}
    };

    // Sort by name using a projection
    ranges::sort(particles, {}, &Particle::name);
    TEST_ENSURES(ranges::equal(particles, {"Electron"_sv, "Muon"_sv, "Neutron"_sv, "Positron"_sv, "Proton"_sv, "Tau"_sv}, {}, &Particle::name));

    // Sort by mass using a projection
    ranges::sort(particles, {}, &Particle::mass);
    TEST_ENSURES(ranges::equal(particles, {0.511, 0.511, 105.66, 938.27, 939.57, 1776.86}, {}, &Particle::mass));
  }

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
