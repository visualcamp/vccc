//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/ranges.hpp"
#include "vccc/string_view.hpp"
#include "vccc/test.hpp"

namespace vccc {
namespace {

int Test() {
  INIT_TEST("vccc::string_view")

  static_assert(std::is_trivially_copyable<string_view>::value, " ");

  {
    std::string s = "hello";
    string_view sv = s;

    static_assert(std::is_constructible<string_view, std::string>::value, " ");
    static_assert(std::is_constructible<string_view, const std::string&>::value, " ");
    static_assert(std::is_constructible<string_view, std::string&&>::value, " ");
    static_assert(std::is_convertible<std::string, string_view>::value, " ");
    static_assert(std::is_convertible<const std::string&, string_view>::value, " ");
    static_assert(std::is_convertible<std::string&&, string_view>::value, " ");

    static_assert(std::is_constructible<string_view, const char*>::value, " ");
    static_assert(std::is_convertible<const char*, string_view>::value, " ");

    // string_view sv2 = std::string();
  }

  {
    std::string s = "hello";
    std::vector<char> v(s.begin(), s.end());

    string_view sv(v);

    TEST_ENSURES(sv == s);
  }

  { // string_view::contains
    // bool contains(basic_string_view x) const noexcept;
    TEST_ENSURES(("https://cppreference.com"_sv.contains("cpp"_sv) == true));
    TEST_ENSURES(("https://cppreference.com"_sv.contains("php"_sv) == false));

    // bool contains(CharT x) const noexcept;
    TEST_ENSURES(("C++23"_sv.contains('+') == true));
    TEST_ENSURES(("C++23"_sv.contains('-') == false));

    // bool contains(const CharT* x) const;
    TEST_ENSURES((string_view("basic_string_view").contains("string") == true));
    TEST_ENSURES((string_view("basic_string_view").contains("String") == false));
  }

  { // basic_string_view::find
    using namespace vccc::literals;

    constexpr auto str{" long long int;"_sv};

    TEST_ENSURES((1 == str.find("long"_sv)));
    TEST_ENSURES((6 == str.find("long"_sv, 2)));
    TEST_ENSURES((0 == str.find(' ')));
    TEST_ENSURES((2 == str.find('o', 1)));
    TEST_ENSURES((2 == str.find("on")));
    TEST_ENSURES((6 == str.find("long double", 5, 4)));
    TEST_ENSURES(str.npos == str.find("float"));
  }

  { // basic_string_view::rfind
    using namespace vccc::literals;
    constexpr auto N = string_view::npos;

    TEST_ENSURES((6 == "AB AB AB"_sv.rfind("AB")));
    TEST_ENSURES((6 == "AB AB AB"_sv.rfind("ABCD", N, 2)));
    TEST_ENSURES((3 == "AB AB AB"_sv.rfind("AB", 5)));
    TEST_ENSURES((0 == "AB CD EF"_sv.rfind("AB", 0)));
    TEST_ENSURES((2 == "B AB AB "_sv.rfind("AB", 2)));
    TEST_ENSURES((N == "B AB AB "_sv.rfind("AB", 1)));
    TEST_ENSURES((5 == "B AB AB "_sv.rfind('A')));
    TEST_ENSURES((4 == "AB AB AB"_sv.rfind('B', 4)));
    TEST_ENSURES((N == "AB AB AB"_sv.rfind('C')));

  }

  { // basic_string_view::find_first_of
    using namespace vccc::literals;
    constexpr auto N = string_view::npos;

    auto is_white_space = [=](const char c) noexcept
    {
      return " \t\n\f\r\v"_sv.find_first_of(c) != N;
    };

    TEST_ENSURES((1 == "alignas"_sv.find_first_of("klmn"_sv)));
    //                   |--------------------------|
    TEST_ENSURES((N == "alignof"_sv.find_first_of("wxyz"_sv)));
    //
    TEST_ENSURES((3 == "concept"_sv.find_first_of("bcde"_sv, /* pos= */ 1)));
    //                  |---------------------------|
    TEST_ENSURES((N == "consteval"_sv.find_first_of("oxyz"_sv, /* pos= */ 2)));
    //
    TEST_ENSURES((6 == "constexpr"_sv.find_first_of('x')));
    //                        |----------------------|
    TEST_ENSURES((N == "constinit"_sv.find_first_of('x')));
    //
    TEST_ENSURES((6 == "const_cast"_sv.find_first_of('c', /* pos= */ 4)));
    //                        |-----------------------|
    TEST_ENSURES((N == "continue"_sv.find_first_of('c', /* pos= */ 42)));
    //
    TEST_ENSURES((5 == "co_await"_sv.find_first_of("cba", /* pos= */ 4)));
    //                       |------------------------|
    TEST_ENSURES((7 == "decltype"_sv.find_first_of("def", /* pos= */ 2, /* count= */ 2)));
    //                         |--------------------|
    TEST_ENSURES((N == "decltype"_sv.find_first_of("def", /* pos= */ 2, /* count= */ 1)));
    //
    TEST_ENSURES((is_white_space(' ') && is_white_space('\r') && !is_white_space('\a')));
  }

  { // basic_string_view::find_last_of
    using namespace vccc::literals;
    constexpr auto N = string_view::npos;
    
    TEST_ENSURES((5 == "delete"_sv.find_last_of("cdef"_sv)));
    //                       |_____________________|
    TEST_ENSURES((N == "double"_sv.find_last_of("fghi"_sv)));
    //
    TEST_ENSURES((0 == "else"_sv.find_last_of("bcde"_sv, 2 /* pos [0..2]: "els" */)));
    //                  |_________________________|
    TEST_ENSURES((N == "explicit"_sv.find_last_of("abcd"_sv, 4 /* pos [0..4]: "expli" */)));
    //
    TEST_ENSURES((3 == "extern"_sv.find_last_of('e')));
    //                     |_____________________|
    TEST_ENSURES((N == "false"_sv.find_last_of('x')));
    //
    TEST_ENSURES((0 == "inline"_sv.find_last_of('i', 2 /* pos [0..2]: "inl" */)));
    //                  |________________________|
    TEST_ENSURES((N == "mutable"_sv.find_last_of('a', 2 /* pos [0..2]: "mut" */)));
    //
    TEST_ENSURES((3 == "namespace"_sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 3 /* "cde" */)));
    //                     |__________________________|
    TEST_ENSURES((N == "namespace"_sv.find_last_of("cdef", 3 /* pos [0..3]: "name" */, 2 /* "cd" */)));
  }

  { // basic_string_view::find_first_not_of
    using namespace vccc::literals;
    TEST_ENSURES((2 == "BCDEF"_sv.find_first_not_of("ABC")));
                    //    ^
    TEST_ENSURES((4 == "BCDEF"_sv.find_first_not_of("ABC", 4)));
                    //      ^
    TEST_ENSURES((1 == "BCDEF"_sv.find_first_not_of('B')));
                    //   ^
    TEST_ENSURES((3 == "BCDEF"_sv.find_first_not_of('D', 2)));
  }

  { // basic_string_view::find_last_not_of
    using namespace vccc::literals;
    TEST_ENSURES((1 == "BCDEF"_sv.find_last_not_of("DEF")));
                    //   ^
    TEST_ENSURES((2 == "BCDEFG"_sv.find_last_not_of("EFG", 3)));
                    //    ^
    TEST_ENSURES((2 == "ABBA"_sv.find_last_not_of('A')));
                    //    ^
    TEST_ENSURES((1 == "ABBA"_sv.find_last_not_of('A', 1)));
                    //    ^
  }

#if __cplusplus >= 201703L
  {
    std::string s = "hello, world!";
    std::vector v(s.begin(), s.end());

    vccc::basic_string_view sv(s.begin(), s.end());
    vccc::basic_string_view sv2(s.data(), s.data() + s.size());
    vccc::basic_string_view sv3(v.begin(), v.end());

    vccc::basic_string_view sv4(s);
    vccc::basic_string_view sv5(v);
  }
#endif

  {
    // Example taken from https://en.cppreference.com/w/cpp/ranges/range_adaptor_closure

    // Define Slice as a range adaptor closure
    struct Slice : vccc::ranges::range_adaptor_closure<Slice> {
      std::size_t start = 0;
      std::size_t end = vccc::string_view::npos;

      constexpr vccc::string_view operator()(vccc::string_view sv) const
      {
        return sv.substr(start, end - start);
      }
    };

    vccc::string_view str = "01234567";

    Slice slicer;
    slicer.start = 1;
    slicer.end = 6;

    auto sv1 = slicer(str); // use slicer as a normal function object
    auto sv2 = str | slicer; // use slicer as a range adaptor closure object

    static_assert(std::is_same<vccc::string_view, decltype(sv1)>::value, "");
    static_assert(std::is_same<vccc::string_view, decltype(sv2)>::value, "");

    TEST_ENSURES(sv1 == "12345");
    TEST_ENSURES(sv2 == "12345");
  }

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
