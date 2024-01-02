//
// Created by YongGyu Lee on 2023/12/23.
//

#include <array>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "vccc/ranges.hpp"
#include "vccc/test.hpp"

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

  return TEST_RETURN_RESULT;
}
