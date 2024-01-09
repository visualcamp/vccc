//
// Created by YongGyu Lee on 2023/12/23.
//

#include <array>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "vccc/ranges.hpp"
#include "vccc/span.hpp"
#include "test_core.hpp"

struct IntLike {
  int v;
};

void swap(IntLike& lhs, int& rhs) {
  std::swap(lhs.v, rhs);
}

void swap(int& lhs, IntLike& rhs) {
  std::swap(lhs, rhs.v);
}

template<typename T, typename... U>
std::vector<T> make_vector(T arg, U... args) {
  return {arg, args...};
}

template<typename T, typename... U>
std::array<T, 1 + sizeof...(U)> make_array(T arg, U... args) {
  return {arg, args...};
}

struct foo {

  std::string get() & { return data; }
  std::string get() && { return std::move(data); }

  std::string data = "Hello";
};

int size(const foo&) { return 1; }

int x;

int* begin(const IntLike&) { return &x; }
int* end(const IntLike&) { return &x + 1; }

struct Bound
{
  int bound;
  bool operator==(int x) const { return x == bound; }
};

int main() {
  INIT_TEST("vccc::ranges")


  {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5};
    vccc::ranges::swap(a, b);

    TEST_ENSURES((a == make_vector(4, 5)));
    TEST_ENSURES(b == make_vector(1, 2, 3));
  }


  {
    std::array<int, 3> a = {1, 2, 3};
    std::array<int, 3> b = {4, 5, 6};
    vccc::ranges::swap(a, b);

    TEST_ENSURES((a == make_array(4, 5, 6)));
    TEST_ENSURES(b == make_array(1, 2, 3));
  }

  {
    int d1[] = {21, 22, 23};
    int d2[] = {24, 25, 26};
    // TODO: Implement swap_ranges
    // vccc::ranges::swap(d1, d2);
  }

  {
    IntLike d1{1};
    int d2{2};
    vccc::ranges::swap(d1, d2);
  }

  {
    int array[] = {1, 2, 3};
    std::vector<int> v = {4, 5, 6};
    auto il = {7};

    TEST_ENSURES((vccc::ranges::size(array) == 3));
    TEST_ENSURES((vccc::ranges::size(v) == 3));
    TEST_ENSURES((vccc::ranges::size(il) == 1));

    foo f;
    vccc::ranges::size(f);

    IntLike i{};
    vccc::ranges::size(i);

    static_assert(std::is_signed<decltype(vccc::ranges::size(v))>::value == false, "");
  }


  {
    int array[] = {1, 2, 3};
    std::vector<int> v = {4, 5, 6};
    auto il = {7};

    TEST_ENSURES((vccc::ranges::empty(array) == false));
    TEST_ENSURES((vccc::ranges::empty(v) == false));
    TEST_ENSURES((vccc::ranges::empty(il) == false));

    foo f;
    vccc::ranges::empty(f);

    IntLike i{};
    vccc::ranges::empty(i);
  }

  {

    for (int i : vccc::ranges::iota_view<int, int>{1, 10})
      std::cout << i << ' ';
    std::cout << '\n';

    for (int i : vccc::views::iota(1, 10))
      std::cout << i << ' ';
    std::cout << '\n';

    static_assert(vccc::views::iota(1, 10).size() == 9, "");

    #if __cplusplus < 201703L
    {
      auto&& r = vccc::views::iota(1, Bound{10});
      auto first = r.begin();
      auto last = r.end();
      for (; first != last; ++first) {
        std::cout << *first << ' ';
      }
      std::cout << '\n';
    }
    #else
    for (auto x : vccc::views::iota(1, Bound{10}))
      std::cout << x << ' ';
    std::cout << '\n';
    #endif

    // for (int i : vccc::views::iota(1) | vccc::views::take(9))
    //   std::cout << i << ' ';
    // std::cout << '\n';

    // vccc::ranges::for_each(vccc::views::iota(1, 10), [](int i)
    // {
    //     std::cout << i << ' ';
    // });
    // std::cout << '\n';
  }

  { // ranges::enable_view
    struct A : vccc::ranges::view_base {};
    struct B : vccc::ranges::view_interface<B> {};
    struct C : vccc::ranges::view_interface<C>, vccc::ranges::view_interface<B> {};

    static_assert(vccc::ranges::enable_view<A>::value, "");
    static_assert(vccc::ranges::enable_view<B>::value, "");
    static_assert(vccc::ranges::enable_view<C>::value == false, "");
    static_assert(vccc::ranges::enable_view<int>::value == false, "");
    static_assert(vccc::ranges::enable_view<void>::value == false, "");
  }

  { // ranges::data
    int arr[] = {1, 2, 3};
    TEST_ENSURES(vccc::ranges::data(arr) == arr);
    auto last = vccc::ranges::end(arr);
    TEST_ENSURES(vccc::ranges::data(arr) + 3 == vccc::ranges::end(arr));
  }

  { // ranges::all_view
    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    for (auto x : vccc::views::all(v)) {
      std::cout << x << ' ';
    }
    std::cout << '\n';
    TEST_ENSURES(vccc::views::all(v).size() == v.size());
    TEST_ENSURES(vccc::views::all(v).data() == v.data());
    TEST_ENSURES(vccc::views::all(v).end() == v.end());
    TEST_ENSURES(vccc::views::all(v).empty() == v.empty());
  }

  { // ranges::single_view
    // Examples got from https://en.cppreference.com/w/cpp/ranges/single_view
    constexpr vccc::ranges::single_view<double> sv1{3.1415};
    static_assert(sv1, "");
    static_assert(not sv1.empty(), "");
    TEST_ENSURES(*sv1.data() == 3.1415);
    TEST_ENSURES(*sv1.begin() == 3.1415);
    TEST_ENSURES(sv1.size() == 1);

    TEST_ENSURES(std::distance(sv1.begin(), sv1.end()) == 1);

    std::string str{"C++20"};
    auto sv2 = vccc::views::single(std::move(str));

    TEST_ENSURES(sv2.data() != nullptr);
    TEST_ENSURES(*sv2.data() == "C++20");
    TEST_ENSURES(str.empty());

    vccc::ranges::single_view<std::tuple<int, double, std::string>>
        sv3{vccc::in_place, 42, 3.14, "Hello"};

    TEST_ENSURES(std::get<0>(sv3[0]) == 42);
    TEST_ENSURES(std::get<1>(sv3[0]) == 3.14);
    TEST_ENSURES(std::get<2>(sv3[0]) == "Hello");
  }

  {
    vccc::ranges::empty_view<long> e;
    static_assert(vccc::ranges::empty(e), "");
    static_assert(0 == e.size(), "");
    static_assert(nullptr == e.data(), "");
    static_assert(nullptr == e.begin(), "");
    static_assert(nullptr == e.end(), "");
  }

  {
    std::vector<int> v{3, 1, 4};
    TEST_ENSURES((vccc::ranges::distance(v.begin(), v.end()) == 3));
    TEST_ENSURES((vccc::ranges::distance(v.end(), v.begin()) == -3));
    TEST_ENSURES((vccc::ranges::distance(v) == 3));

    std::forward_list<int> l{2, 7, 1};
    // auto size = vccc::ranges::size(l); // error: not a sizable range

    auto size = vccc::ranges::distance(l); // OK, but aware O(N) complexity
    TEST_ENSURES(size == 3);
  }

  {
    std::multimap<int, char> mm{{4, 'a'}, {3, '-'}, {4, 'b'}, {5, '-'}, {4, 'c'}};
    auto mutate = [](auto& v) {
      v += 'A' - 'a';
    };

#if __cplusplus < 201703L
    auto print = [](const auto& rem, const auto& mm) {
      std::cout << rem << "{ ";
      for (const auto& kv : mm)
        std::cout << '{' << kv.first << ",'" << kv.second << "'} ";
      std::cout << "}\n";
    };

    print("Before: ", mm);
    auto r = mm.equal_range(4);
    for (auto& p : vccc::ranges::make_subrange(r.first, r.second)) {
      mutate(p.second);
    }
    print("After:  ", mm);
#else
    auto print = [](const auto& rem, const auto& mm) {
      std::cout << rem << "{ ";
      for (const auto& [k, v] : mm)
        std::cout << '{' << k << ",'" << v << "'} ";
      std::cout << "}\n";
    };

    print("Before: ", mm);
    auto [first, last] = mm.equal_range(4);
    for (auto& [_, v] : vccc::ranges::make_subrange(first, last)) {
      mutate(v);
    }
    std::cout << "After:  " << "{ ";
    for (const auto& [k, v] : vccc::ranges::subrange(mm))
        std::cout << '{' << k << ",'" << v << "'} ";
    std::cout << "}\n";
#endif
  }

  {
    std::vector<int> v = {1, 2, 3};

    for (auto x : vccc::views::take(v, 2)) {
      std::cout << x << ' ';
    }
    std::cout << '\n';

    for (auto x : vccc::views::take(v, 982)) {
      std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << __FILE__ << ", " << __LINE__ << ": ";
    for (auto x : vccc::views::iota(10, 20) | vccc::views::take(7)) {
      std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << __FILE__ << ", " << __LINE__ << ": ";
    vccc::span<int> s = v;
    auto r = vccc::views::take(s, 4) | vccc::views::take(2) | vccc::views::take(2) | vccc::views::take(999);
    for (auto x : r) {
      std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << __FILE__ << ", " << __LINE__ << ": ";
    for (const auto& x : vccc::views::take(vccc::ranges::make_subrange(s.begin() + 1, s.end()), 10)) {
      std::cout << x << ' ';
    }
    std::cout << '\n';
  }

  return TEST_RETURN_RESULT;
}