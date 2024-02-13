//
// Created by YongGyu Lee on 2023/12/23.
//

#include <array>
#include <cctype>
#include <forward_list>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "vccc/algorithm.hpp"
#include "vccc/iterator.hpp"
#include "vccc/ranges.hpp"
#include "vccc/span.hpp"
#include "vccc/string_view.hpp"
#include "vccc/type_traits.hpp"
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

char rot13a(const char x, const char a)
{
  return a + (x - a + 13) % 26;
}

char rot13(const char x)
{
  if ('Z' >= x and x >= 'A')
    return rot13a(x, 'A');

  if ('z' >= x and x >= 'a')
    return rot13a(x, 'a');

  return x;
}

int main() {
  INIT_TEST("vccc::ranges")
  using namespace vccc::string_view_literals;


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
    vccc::ranges::swap(d1, d2);
    TEST_ENSURES(vccc::ranges::equal(d1, vccc::span<const int>({24, 25, 26})));
    TEST_ENSURES(vccc::ranges::equal(d2, vccc::span<const int>({21, 22, 23})));
  }

  {
    IntLike d1{1};
    int d2{2};
    vccc::ranges::swap(d1, d2);
  }

  { // ranges::size
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


  { // ranges::empty
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

  { // ranges::rbegin
    std::cout << "Line " << __LINE__ << ", ranges::rbegin: \n";

    std::vector<int> v = {3, 1, 4};
    auto vi = vccc::ranges::rbegin(v);
    TEST_ENSURES(*vi == 4);
    *vi = 42; // OK
    TEST_ENSURES(v.back() == 42);

    int a[] = {-5, 10, 15};
    auto ai = vccc::ranges::rbegin(a);
    TEST_ENSURES(*ai == 15);
    *ai = 42; // OK
    TEST_ENSURES(a[2] == 42);

    // auto x_x = std::ranges::rbegin(std::vector{6, 6, 6});
    // ill-formed: the argument is an rvalue (see Notes above)

    auto si = vccc::ranges::rbegin(vccc::span<int>(a)); // OK
    static_assert(vccc::ranges::enable_borrowed_range<
        std::remove_cv_t<decltype(vccc::span<int>(a))>>::value, "");
    *si = 52; // OK
    TEST_ENSURES(a[2] == 52);
  }

  { // ranges::rend
    std::cout << "Line " << __LINE__ << ", ranges::rend: \n";

    std::vector<int> v = {3, 1, 4};
    namespace ranges = vccc::ranges;
    TEST_ENSURES((ranges::find(ranges::rbegin(v), ranges::rend(v), 5) == ranges::rend(v)));

    int a[] = {5, 10, 15};
    TEST_ENSURES((ranges::find(ranges::rbegin(a), ranges::rend(a), 5) != ranges::rend(a)));
  }

  { // ranges::iota_view, views::iota
    std::cout << "Line " << __LINE__ << ", ranges::iota_view, views::iota: \n";

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

  { // ranges::take

    constexpr char pi[]{'3', '.', '1', '4', '1', '5', '9', '2', '6', '5'};

    vccc::ranges::for_each(pi | vccc::views::take(6), [](char x){ std::cout << x; });
    std::cout << '\n';

    vccc::ranges::for_each(vccc::ranges::make_take_view(pi, 42), [](char x){ std::cout << x; });
    std::cout << '\n';

#if __cplusplus >= 201703L
    vccc::ranges::for_each(vccc::ranges::take_view(pi, 42), [](char x){ std::cout << x; });
    std::cout << '\n';
#endif

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

  { // transform_view
    std::string s = "The length of this string is 42 characters";
    auto tv = vccc::ranges::make_transform_view(s, [](char c) -> char {
      return std::toupper(c);
    });

    TEST_ENSURES(vccc::ranges::equal(tv, std::string("THE LENGTH OF THIS STRING IS 42 CHARACTERS")));
    TEST_ENSURES(tv.size() == 42);

    std::cout << __FILE__ << ", " << __LINE__ << ": ";
    for (auto x : tv)
      std::cout << x;
    std::cout << "\nsize = " << tv.size() << '\n';

    auto show = [](const unsigned char x) { std::putchar(x); };

    std::string in{"cppreference.com\n"};
    vccc::ranges::for_each(in, show);

    vccc::ranges::for_each(in | vccc::views::transform(rot13), show);
    TEST_ENSURES(vccc::ranges::equal(in | vccc::views::transform(rot13), std::string("pccersrerapr.pbz\n")));


    std::string out;
    vccc::ranges::copy(vccc::views::transform(in, rot13), std::back_inserter(out));
    vccc::ranges::for_each(out, show);
    TEST_ENSURES(out == "pccersrerapr.pbz\n");
    vccc::ranges::for_each(out | vccc::views::transform(rot13), show);
    TEST_ENSURES(vccc::ranges::equal(out | vccc::views::transform(rot13), std::string("cppreference.com\n")));
  }
  {
    const auto v = { 0, 1, 2, 3, 4 };
    auto x2 = [](int x) { return x << 1; };
    auto tv = vccc::ranges::make_transform_view(v, x2);
    std::ostream_iterator<int> ostr{ std::cout, " " };

    TEST_ENSURES(vccc::ranges::equal(tv.base(), v));
    TEST_ENSURES(vccc::ranges::equal(tv, std::vector<int>{0, 2, 4, 6, 8}));
    vccc::ranges::copy(v, ostr), std::cout << '\n';
    vccc::ranges::copy(tv.base(), ostr), std::cout << '\n';
    vccc::ranges::copy(tv, ostr), std::cout << '\n';
  }

  { // ranges::to < C >
    int arr[] = {1, 2, 3, 4, 5};

    auto v = vccc::ranges::to<std::vector<int>>(arr);
    static_assert(std::is_same<std::vector<int>, decltype(v)>::value, "");
    TEST_ENSURES(vccc::ranges::equal(v, arr));
    TEST_ENSURES(v.size() == 5);

    auto li = vccc::ranges::to<std::list<int>>(arr);
    auto li2 = vccc::ranges::to<std::list<int>>(v);
    auto li3 = vccc::ranges::to<std::list<int>>(li2);

    TEST_ENSURES(vccc::ranges::equal(li, arr));
    TEST_ENSURES(vccc::ranges::equal(li2, arr));
    TEST_ENSURES(vccc::ranges::equal(li3, arr));

    auto chain = vccc::views::iota(0, 10)
               | vccc::views::take(4)
               | vccc::views::transform([](auto x) { return -x; });
    auto v2 = vccc::ranges::to<std::vector<int>>(chain);
    TEST_ENSURES(vccc::ranges::equal(v2, std::vector<int>{0, -1, -2, -3}));

  }

  { // ranges::to< template<typename...> C >
    int arr[] = {1, 2, 3, 4, 5};

    auto v = vccc::ranges::to<std::vector>(arr);
    static_assert(std::is_same<std::vector<int>, decltype(v)>::value, "");
    TEST_ENSURES(vccc::ranges::equal(v, arr));
    TEST_ENSURES(v.size() == 5);

    auto li = vccc::ranges::to<std::list>(arr);
    auto li2 = vccc::ranges::to<std::list>(v);
    auto li3 = vccc::ranges::to<std::list>(li2);

    TEST_ENSURES(vccc::ranges::equal(li, arr));
    TEST_ENSURES(vccc::ranges::equal(li2, arr));
    TEST_ENSURES(vccc::ranges::equal(li3, arr));

    auto chain = vccc::views::iota(0, 10)
               | vccc::views::take(4)
               | vccc::views::transform([](auto x) { return -x; });
    auto v2 = vccc::ranges::to<std::vector>(chain);
    TEST_ENSURES(vccc::ranges::equal(v2, std::vector<int>{0, -1, -2, -3}));


    std::pair<int, std::string> parr[] = {{1, "one"}, {2, "two"}};
    auto pv = vccc::ranges::to<std::vector>(parr);
    static_assert(std::is_same<std::vector<std::pair<int, std::string>>, decltype(pv)>::value, "");
    TEST_ENSURES(pv.size() == 2);
    TEST_ENSURES(pv[0].second == "one");
    TEST_ENSURES(pv[1].second == "two");

    auto m = vccc::ranges::to<std::map>(parr, std::greater<>{});
    static_assert(std::is_same<std::map<int, std::string, std::greater<>>, decltype(m)>::value, "");
    TEST_ENSURES(m.size() == 2);
    TEST_ENSURES(m[1] == "one");
    TEST_ENSURES(m[2] == "two");
    TEST_ENSURES(m.begin()->second == "two");
  }

  { // ranges::to (range adaptor closure)
    auto vec = vccc::views::iota(1, 5)
             | vccc::views::transform([](auto const v) { return v * 2; })
             | vccc::ranges::to<std::vector>();

    static_assert(std::is_same<std::vector<int>, decltype(vec)>::value, "");
    TEST_ENSURES(vec.size() == 4);
    TEST_ENSURES(vec[0] == 2);
    TEST_ENSURES(vec[1] == 4);
    TEST_ENSURES(vec[2] == 6);
    TEST_ENSURES(vec[3] == 8);

    auto lst = vec | vccc::views::take(3) | vccc::ranges::to<std::list<double>>();
    static_assert(std::is_same<std::list<double>, decltype(lst)>::value, "");
    TEST_ENSURES(lst.size() == 3);
    TEST_ENSURES(*vccc::ranges::next(lst.begin(), 0) == 2);
    TEST_ENSURES(*vccc::ranges::next(lst.begin(), 1) == 4);
    TEST_ENSURES(*vccc::ranges::next(lst.begin(), 2) == 6);
  }

  { // ranges::cbegin
    std::vector<int> v{3, 1, 4};
    auto vi = vccc::ranges::cbegin(v);
    TEST_ENSURES(3 == *vi);
    ++vi; // OK, constant-iterator object is mutable
    TEST_ENSURES(1 == *vi);
    // *vi = 13; // Error: constant-iterator points to an immutable element

    int a[]{3, 1, 4};
    auto ai = vccc::ranges::cbegin(a); // cbegin works with C-arrays as well
    TEST_ENSURES(3 == *ai and *(ai + 1) == 1);
  }

  { // ranges::cend
    std::vector<int> vec = {3, 1, 4};
    int arr[] = {5, 10, 15};

    TEST_ENSURES(vccc::ranges::find(vec, 5) == vccc::ranges::cend(vec));
    TEST_ENSURES(vccc::ranges::find(arr, 5) != vccc::ranges::cend(arr));
    TEST_ENSURES(vccc::ranges::cend(vec) == vccc::ranges::find(vec, 5) );
    TEST_ENSURES(vccc::ranges::cend(arr) != vccc::ranges::find(arr, 5) );
  }

  { // views::as_const
    int x[] = {1, 2, 3, 4, 5};
    auto v1 = x | vccc::views::take(2);
    TEST_ENSURES(v1.back() == 2);
    static_assert(vccc::detail::is_post_incrementable<decltype(v1[0])>::value, "");

    auto v2 = x | vccc::views::take(2) | vccc::views::as_const;

    TEST_ENSURES(v2.back() == 2);
    static_assert(vccc::detail::is_post_incrementable<decltype(v2[0])>::value == false, "");
  }

  { // views::enumerate
    constexpr static auto v = {'A', 'B', 'C', 'D'};

#if __cplusplus < 201703L
    for (const auto p : vccc::views::enumerate(v))
      std::cout << '(' << std::get<0>(p) << ':' << std::get<1>(p) << ") ";
#else
    for (auto const [index, letter] : vccc::views::enumerate(v))
      std::cout << '(' << index << ':' << letter << ") ";
#endif
    std::cout << std::endl;
    TEST_ENSURES(vccc::views::enumerate(v).size() == 4);
    TEST_ENSURES(vccc::views::enumerate(v).begin().index() == 0);
    TEST_ENSURES((vccc::views::enumerate(v).end() - 1).index() == 3);

    auto m = v | vccc::views::enumerate | vccc::ranges::to<std::map>();

#if __cplusplus < 201703L
    for (const auto kv : m)
      std::cout << '[' << std::get<0>(kv) << "]:" << std::get<1>(kv) << ' ';
#else
    for (auto const [key, value] : m)
      std::cout << '[' << key << "]:" << value << ' ';
#endif
    std::cout << '\n';
    TEST_ENSURES(m.size() == 4);
    TEST_ENSURES(m[0] == 'A');
    TEST_ENSURES(m[1] == 'B');
    TEST_ENSURES(m[2] == 'C');
    TEST_ENSURES(m[3] == 'D');


    std::vector<int> numbers{1, 3, 5, 7};

#if __cplusplus < 201703L
    for (const auto i_n : vccc::views::enumerate(numbers)) {
      ++std::get<1>(i_n);
      std::cout << numbers[std::get<0>(i_n)] << ' ';
    }
#else
    for (auto const [index, num] : vccc::views::enumerate(numbers)) {
      ++num; // the type is int&
      std::cout << numbers[index] << ' ';
    }
#endif
    std::cout << '\n';
  }

  { // filter_view
    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };

    for (int i : vccc::views::iota(0, 6)
               | vccc::views::filter(even)
               | vccc::views::transform(square))
      std::cout << i << ' ';
    std::cout << '\n';

    auto v = vccc::views::iota(0, 6)
           | vccc::views::filter(even)
           | vccc::views::transform(square)
           | vccc::ranges::to<std::vector>();
    TEST_ENSURES((vccc::ranges::equal(v , std::vector<int>{0, 4, 16})));
  }

  { // enumerate_view
    std::cout << "Line " << __LINE__ << ", enumerate_view: \n";

    const std::vector<std::tuple<int, char, std::string>> vt
    {
          {1, 'A', "a"},
          {2, 'B', "b"},
          {3, 'C', "c"},
          {4, 'D', "d"},
          {5, 'E', "e"},
    };

    // 1 2 3 4 5
    for (int const e : vccc::views::elements<0>(vt))
      std::cout << e << ' ';
    std::cout << '\n';
    TEST_ENSURES((vccc::ranges::equal(vccc::views::elements<0>(vt), vccc::views::iota(1, 6))));

    // A B C D E
    for (char const e : vt | vccc::views::elements<1>)
      std::cout << e << ' ';
    std::cout << '\n';
    TEST_ENSURES((vccc::ranges::equal(vt | vccc::views::elements<1>, vccc::views::iota('A', 'F'))));

    // a b c d e
    for (std::string const& e : vccc::views::elements<2>(vt))
      std::cout << e << ' ';
    std::cout << '\n';
    TEST_ENSURES((vccc::ranges::equal(
        vccc::views::elements<2>(vt),
        vccc::views::iota('a', 'f') | vccc::views::transform([](char c) { return std::string(1, c); })
    )));
  }

  { // ranges::cartesian_product
    std::cout << "Line " << __LINE__ << ", cartesian_product, cartesian_product_view: \n";

    const auto x = std::array<char, 2>{'A', 'B'};
    const auto y = std::array<int, 3>{1, 2, 3};
    const auto z = std::array<std::string, 4>{"a", "b", "c", "d"};

    namespace ranges = vccc::ranges;
    namespace views = vccc::views;

    using XAT = views::all_t<decltype((x))>;
    using YAT = views::all_t<decltype((y))>;
    using ZAT = views::all_t<decltype((z))>;
    ranges::cartesian_product_view<XAT, YAT, ZAT> cpv(x, y, z);

    auto first = cpv.begin();
    auto last = cpv.end();
    TEST_ENSURES((last - first) == 24);

#if __cplusplus < 201703L
    for (const auto& t : views::cartesian_product(x, y, z)) {
      std::cout << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t) << '\n';
    }
#else
    for (const auto& [a, b, c] : views::cartesian_product(x, y, z)) {
      std::cout << a << ' ' << b << ' ' << c << '\n';
    }
#endif
    auto a = first + 10;
    auto b = first + 9;
    TEST_ENSURES((a - b) == 1);
    TEST_ENSURES((b - a) == -1);

    { // cartesian_product_view::size
      constexpr static auto w = {1};
      constexpr static auto x = {2, 3};
      constexpr static auto y = {4, 5, 6};
      constexpr static auto z = {7, 8, 9, 10, 11, 12, 13};
      auto v = views::cartesian_product(w, x, y, z);
      TEST_ENSURES(v.size() == w.size() * x.size() * y.size() * z.size());
      TEST_ENSURES(v.size() == 42);
    }
  }

  { // views::counted
    const int a[]{1, 2, 3, 4, 5, 6, 7};
    TEST_ENSURES((vccc::ranges::equal(vccc::views::counted(a, 3), vccc::views::iota(1) | vccc::views::take(3))));

    const auto il = {1, 2, 3, 4, 5};
    TEST_ENSURES((vccc::ranges::equal( vccc::views::counted(il.begin() + 1, 3), vccc::views::iota(2) | vccc::views::take(3))));
  }

  { // ranges::drop_view, views::drop
    std::cout << "Line " << __LINE__ << ", ranges::drop_view, views::drop: \n";

    const auto nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    TEST_ENSURES((vccc::ranges::equal(vccc::ranges::make_drop_view(nums, 2), vccc::views::iota(3, 10))));

    TEST_ENSURES((vccc::ranges::equal(nums | vccc::views::drop(2), vccc::views::iota(3, 10))));

    TEST_ENSURES((vccc::ranges::equal(vccc::views::iota(1, 10) | vccc::views::drop(2), vccc::views::iota(3, 10))));
  }

  { // ranges::repeat_view, views::drop
    std::cout << "Line " << __LINE__ << ", ranges::repeat_view, views::drop: \n";

    TEST_ENSURES((vccc::ranges::equal(
        vccc::views::repeat("C++"_sv, 3),
        vccc::span<const vccc::string_view>{"C++"_sv, "C++"_sv, "C++"_sv}
        )));

    TEST_ENSURES((vccc::ranges::equal(
        vccc::views::repeat("C++"_sv) | vccc::views::take(3),
        vccc::span<const vccc::string_view>{"C++"_sv, "C++"_sv, "C++"_sv}
        )));
  }

  { // ranges::join_view, views::join
    std::cout << "Line " << __LINE__ << ", ranges::join_view, views::join: \n";


    const auto bits = {"https:"_sv, "//"_sv, "cppreference"_sv, "."_sv, "com"_sv};
    TEST_ENSURES((vccc::ranges::equal(bits | vccc::views::join, "https://cppreference.com"_sv)));

    const std::vector<std::vector<int>> v{{1, 2}, {3, 4, 5}, {6}, {7, 8, 9}};
    TEST_ENSURES((vccc::ranges::equal(vccc::views::join(v), vccc::span<const int>{1, 2, 3, 4, 5, 6, 7, 8, 9})));

    struct Person { int age; std::string name; };

    auto f = [](std::vector<Person>& v){
      return v | vccc::views::transform([](auto& p) -> std::string& { return p.name; })
               | vccc::views::join; // OK
    };

    std::vector<Person> pv = {{10, "john"}, {20, "james"}};
    TEST_ENSURES((vccc::ranges::equal(f(pv), "johnjames"_sv)));
  }

  { // ranges::istream_view, views::istream
    std::cout << "Line " << __LINE__ << ", ranges::istream_view, views::istream: \n";

    auto words = std::istringstream{"today is yesterday's tomorrow"};
    auto s = vccc::views::istream<std::string>(words);

    for (auto it = s.begin(); it != s.end(); ++it)
      std::cout << std::quoted(*it, '/') << ' ';
    std::cout << '\n';

    auto floats = std::istringstream{"1.1  2.2\t3.3\v4.4\f55\n66\r7.7  8.8"};
    vccc::ranges::copy(
        vccc::views::istream<float>(floats),
        std::ostream_iterator<float>{std::cout, ", "}
    );
    std::cout << '\n';

  }

  { // ranges::split_view, views::split
    std::cout << "Line " << __LINE__ << ", ranges::split_view, views::split: \n";
    {
      auto w1 = vccc::ranges::make_split_view("a::b::c"_sv, "::"_sv);
      static_assert(std::is_same<
          decltype(w1),
          vccc::ranges::split_view<vccc::string_view, vccc::string_view> >::value, "");

      auto w2 = vccc::ranges::make_split_view("x,y,z"_sv, ',');
      static_assert(std::is_same<
          decltype(w2),
          vccc::ranges::split_view<vccc::string_view, vccc::ranges::single_view<char>> >::value, "");
    }

    {
      auto view = vccc::views::iota(1, 20)
                | vccc::views::transform([](int x) { return x % 5; });
      auto splitts = vccc::views::split(view, 0); // (2)

      auto it = splitts.begin();
      TEST_ENSURES(vccc::ranges::equal(*it++, {1, 2, 3, 4}));
      TEST_ENSURES(vccc::ranges::equal(*it++, {1, 2, 3, 4}));
      TEST_ENSURES(vccc::ranges::equal(*it++, {1, 2, 3, 4}));
      TEST_ENSURES(vccc::ranges::equal(*it++, {1, 2, 3, 4}));
      TEST_ENSURES(it == splitts.end());

      for (const auto& split : splitts) {
        std::cout << "{ ";
        vccc::ranges::copy(split, std::ostream_iterator<int>(std::cout, " "));
        std::cout << "} ";
      }
      std::cout << '\n';
    }

    {
      const std::vector<int> nums{1, -1, -1, 2, 3, -1, -1, 4, 5, 6};
      const std::array<int, 2> delim{-1, -1};
      auto splitter = vccc::views::split(nums, delim); // (3)

      auto it = splitter.begin();
      TEST_ENSURES(vccc::ranges::equal(*it++, {1}));
      TEST_ENSURES(vccc::ranges::equal(*it++, {2, 3}));
      TEST_ENSURES(vccc::ranges::equal(*it++, {4, 5, 6}));
      TEST_ENSURES(it == splitter.end());

      for (const auto& split : splitter) {
        std::cout << "{ ";
        vccc::ranges::copy(split, std::ostream_iterator<int>(std::cout, " "));
        std::cout << "} ";
      }
      std::cout << '\n';
    }

    {
      const vccc::string_view JupiterMoons{"Callisto, Europa, Ganymede, Io, and 91 more"};
      const vccc::string_view delim{", "};
      auto moons_extractor = vccc::ranges::make_split_view(JupiterMoons, delim); // (3)
      auto is_moon = vccc::views::filter([](auto str) {
          return std::isupper(str[0]);
      });

      auto splitted = moons_extractor | is_moon;
      auto it = splitted.begin();
      TEST_ENSURES(vccc::ranges::equal(*it++, "Callisto"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "Europa"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "Ganymede"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "Io"_sv));
      TEST_ENSURES(it == (moons_extractor | is_moon).end());

      std::cout << "Some moons of Jupiter: ";
      for (const auto moon : moons_extractor | is_moon)
        std::cout << vccc::string_view(moon) << ' ';
      std::cout << '\n';
    }

    {
      constexpr auto words{"Hello^_^C++^_^20^_^!"_sv};
      constexpr auto delim{"^_^"_sv};

      auto splitted = vccc::views::split(words, delim);
      auto it = splitted.begin();
      TEST_ENSURES(vccc::ranges::equal(*it++, "Hello"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "C++"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "20"_sv));
      TEST_ENSURES(vccc::ranges::equal(*it++, "!"_sv));
      TEST_ENSURES(it == vccc::views::split(words, delim).end());

      for (const auto word : vccc::views::split(words, delim))
          std::cout << std::quoted(std::string(vccc::string_view(word))) << ' ';
      std::cout << '\n';
    }

    {
      const vccc::string_view keywords{"this throw true try typedef typeid"};
      const std::vector<vccc::string_view> splitted{"this", "throw", "true", "try", "typedef", "typeid"};

      auto split_view = vccc::ranges::make_split_view(keywords, ' ');
      TEST_ENSURES(vccc::ranges::equal(split_view, splitted, vccc::ranges::equal_to{}, [](const auto& sub) { return vccc::string_view{sub}; }));

      std::cout << "base() = " << std::quoted(std::string(split_view.base())) << "\n"
                   "substrings: ";
      for (auto split : split_view)
        std::cout << std::quoted(std::string(vccc::string_view{split})) << ' ';
      std::cout << '\n';
    }

    {
      const vccc::string_view keywords{"bitand bitor bool break"};
      auto kw = vccc::ranges::make_split_view(keywords, ' ');
      const auto count = vccc::ranges::distance(kw.begin(), kw.end());
      std::cout << "Words count: " << count << '\n';
      TEST_ENSURES(count == 4);
    }
  }

  { // common_view, common
    {
      auto v1 = {1, 2, 3, 4, 5};
      auto i1 = vccc::counted_iterator<decltype(v1)::iterator>{v1.begin(), vccc::ssize(v1)};
      auto r1 = vccc::ranges::subrange<decltype(i1), vccc::default_sentinel_t>{i1, vccc::default_sentinel};
      //  auto e1 = std::accumulate(r1.begin(), r1.end(), 0); // error: "common range" required
      auto c1 = vccc::ranges::make_common_view(r1);
      TEST_ENSURES((std::accumulate(c1.begin(), c1.end(), 0) == 15));
      //
      // // inherited from ranges::view_interface:
      TEST_ENSURES(c1.front() == 1);
      TEST_ENSURES(c1.back() == 5);
      std::cout << "c1.data(): " << c1.data() << '\n';
      TEST_ENSURES(c1[0] == 1);

      auto v2 = std::list<int>{1, 2, 3, 4, 5};
      auto i2 = vccc::counted_iterator<decltype(v2)::iterator>{v2.begin(), vccc::ssize(v2)};
      auto r2 = vccc::ranges::subrange<decltype(i2), vccc::default_sentinel_t>{i2, vccc::default_sentinel};
      //  auto e2 = std::accumulate(r2.begin(), r2.end(), 0); // error: "common range" required
      auto c2 = vccc::ranges::make_common_view(r2);
      TEST_ENSURES((std::accumulate(c2.begin(), c2.end(), 0) == 15));

      // inherited from ranges::view_interface:
      std::cout << "c2.front(): " << c2.front() << '\n';
      TEST_ENSURES(c2.front() == 1);
      //  auto e3 = c2.back(); // error: "bidirectional range" required
      //  auto e4 = c2.data(); // error: "contiguous range" required
      //  auto e5 = c2[0];     // error: "random access range" required
    }

    {
      std::string str { "C++20" };
      auto view = vccc::views::common(str);

      std::string copy_of_str = view.base();
      TEST_ENSURES(copy_of_str == "C++20");
      TEST_ENSURES(view.base() == "C++20");

      std::string move_str = std::move(view.base());
      TEST_ENSURES(move_str == "C++20");
      TEST_ENSURES(view.base() == vccc::string_view{});
    }

    {
      constexpr auto common = vccc::views::iota(1)
                          | vccc::views::take(3)
                          | vccc::views::common
                          ;
      int i = 0;
      auto first = common.begin(), last = common.end();
      for (int e : common)
        std::cout << (i++ ? " + " : "") << e;

      std::cout << " = " << std::accumulate(common.begin(), common.end(), 0) << '\n';
      TEST_ENSURES((std::accumulate(common.begin(), common.end(), 0) == 6));
    }

    {
      constexpr int n{4};

      constexpr auto v1 = vccc::views::iota(1)
                        | vccc::views::take(n)
                        | vccc::views::common
                        ;
      constexpr auto v2 = vccc::views::iota(2)
                        | vccc::views::take(n)
                        ;
      const int product = std::inner_product(v1.begin(), v1.end(),
                                             v2.begin(),
                                             0);
      TEST_ENSURES(product == 40);
    }

    {
      constexpr static auto v1 = {1, 2, 3, 4, 5};
      constexpr auto common1 { v1 | vccc::views::common };
      TEST_ENSURES(common1.size() == 5);

      constexpr auto take3 { v1 | vccc::views::reverse | vccc::views::take(3) };
      constexpr auto common2 { take3 | vccc::views::common };
      static_assert(common2.size() == 3, "");

      constexpr static auto v2 = { "^"_sv, "v"_sv, "<"_sv, ">"_sv };
      TEST_ENSURES(vccc::ranges::views::common(v2).size() == 4);
    }
  }

  { // ranges::common_view, views::common
    std::cout << "Line " << __LINE__ << ", ranges::common_view, views::common: \n";

    static constexpr auto il = {3, 1, 4, 1, 5, 9};

    auto rv = vccc::ranges::make_reverse_view(il);
    for (int i : rv)
      std::cout << i << ' ';
    std::cout << '\n';
    TEST_ENSURES((vccc::ranges::equal(rv, {9, 5, 1, 4, 1, 3})));

    for (int i : il | vccc::views::reverse)
      std::cout << i << ' ';
    std::cout << '\n';
    TEST_ENSURES((vccc::ranges::equal(il | vccc::views::reverse, {9, 5, 1, 4, 1, 3})));

    // operator[] is inherited from std::view_interface
    for (auto i{0U}; i != rv.size(); ++i)
      std::cout << rv[i] << ' ';
    std::cout << '\n';
  }

  { // ranges::join_with_view, views::join_with
    std::cout << "Line " << __LINE__ << ", ranges::join_with_view, views::join_with: \n";

    std::vector<vccc::string_view> v{"This"_sv, "is"_sv, "a"_sv, "test."_sv};
    auto joined = vccc::views::join_with(v, ' ');

    TEST_ENSURES((vccc::ranges::equal(joined, "This is a test."_sv)));
    auto s = joined | vccc::ranges::to<std::string>();
    static_assert(vccc::same_as<std::string, decltype(s)>::value, "");
    TEST_ENSURES(s == "This is a test.");
  }

  {
    std::string a = "hello, ";
    std::vector<char> b = {'w', 'o', 'r', 'l', 'd', '!'};

    auto catted = vccc::views::concat(a, b);

    for (const auto c : catted) {
      std::cout << c;
    }
    std::cout << '\n';

    std::vector<int> v1 = {1, 2, 3, 4, 5};

    for (const auto x : vccc::views::concat(v1, vccc::views::iota(100) | vccc::views::take(10) | vccc::views::common )) {
      std::cout << x;
    }
    std::cout << '\n';
  }

  return TEST_RETURN_RESULT;
}
