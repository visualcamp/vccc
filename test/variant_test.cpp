//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "vccc/algorithm.hpp"
#include "vccc/functional.hpp"
#include "vccc/ranges.hpp"
#include "vccc/string_view.hpp"
#include "vccc/variant.hpp"
#include "test_core.hpp"

namespace vccc {
namespace {

int x_dtor = 0;
int y_dtor = 0;

template<typename... Ts>
struct overloaded_t;

template<typename T>
struct overloaded_t<T> : T {
  overloaded_t(T x) : T(std::move(x)) {}
  using T::operator();
};
template<typename T, typename... Ts>
struct overloaded_t<T, Ts...> : T, overloaded_t<Ts...> {
  overloaded_t(T x, Ts... y)
      : T(std::move(x)), overloaded_t<Ts...>(std::move(y)...) {}

  using T::operator();
  using overloaded_t<Ts...>::operator();
};

template<class... Ts>
auto overloaded(Ts&&... t) {
  return overloaded_t<std::remove_reference_t<Ts>...>(std::forward<Ts>(t)...);
}

int Test() {
  INIT_TEST("vccc::variant")

  { // constructor
    using vector_t = std::vector<int>;

    variant<int, std::string> var0;
    TEST_ENSURES(holds_alternative<int>(var0) &&
                 var0.index() == 0 &&
                 std::get<int>(var0) == 0);

    // initializes first alternative with std::string{"STR"};
    variant<std::string, int> var1{"STR"};
    TEST_ENSURES(var1.index() == 0);
    TEST_ENSURES(std::get<std::string>(var1) == "STR");

    // initializes second alternative with int == 42;
    variant<std::string, int> var2{42};
    TEST_ENSURES(holds_alternative<int>(var2));
    TEST_ENSURES(std::get<int>(var2) == 42);

    // initializes first alternative with std::string{4, 'A'};
    variant<std::string, vector_t, float> var3{vccc::in_place_type<std::string>, 4, 'A'};
    TEST_ENSURES(var3.index() == 0);
    TEST_ENSURES(std::get<std::string>(var3) == "AAAA");

    // initializes second alternative with std::vector{1,2,3,4,5};
    variant<std::string, vector_t, char> var4{vccc::in_place_type<vector_t>, {1, 2, 3, 4, 5}};
    TEST_ENSURES(var4.index() == 1);
    TEST_ENSURES(ranges::equal(std::get<vector_t>(var4), views::iota(1) | views::take(5)));

    // initializes first alternative with std::string{"ABCDE", 3};
    variant<std::string, vector_t, bool> var5 {vccc::in_place_index<0>, "ABCDE", 3};
    TEST_ENSURES(var5.index() == 0);
    TEST_ENSURES(std::get<std::string>(var5) == "ABC");

    // initializes second alternative with std::vector(4, 42);
    variant<std::string, vector_t, char> var6 {vccc::in_place_index<1>, 4, 42};
    TEST_ENSURES(holds_alternative<vector_t>(var6));
    TEST_ENSURES(ranges::equal(std::get<vector_t>(var6), views::repeat(42, 4) ));
  }

  { // destructor
    struct X { ~X() { ++x_dtor; } };
    struct Y { ~Y() { ++y_dtor; } };
    TEST_ENSURES(x_dtor == 0 && y_dtor == 0);

    { variant<X,Y> var; }
    TEST_ENSURES(x_dtor == 1 && y_dtor == 0);

    { variant<X,Y> var{ in_place_index_t<1>{} }; } // constructs var(Y)
    TEST_ENSURES(x_dtor == 1 && y_dtor == 1);
  }

  { // operator=
    struct string_visitor {
      string_view operator()(int x) const { return {}; }
      string_view operator()(const std::string& x) const { return x; }
    };
    struct int_visitor {
      int operator()(int x) const { return x; }
      int operator()(const std::string& x) const { return -9999; }
    };

    variant<int, std::string> a{2017};
    variant<int, std::string> b{"CppCon"};

    TEST_ENSURES(a.visit(int_visitor{}) == 2017);
    visit(int_visitor{}, b);
    // TEST_ENSURES(b.visit(string_visitor{}) == "CppCon");

    TEST_ENSURES(vccc::visit(int_visitor{}, a) == 2017);
    TEST_ENSURES(vccc::visit(string_visitor{}, b) == "CppCon");

    a = b;
    TEST_ENSURES(a == b);
    TEST_ENSURES(a.visit(string_visitor{}) == "CppCon");
    TEST_ENSURES(b.visit(string_visitor{}) == "CppCon");

    a = std::move(b);
    TEST_ENSURES(a != b);
    TEST_ENSURES(a.visit(string_visitor{}) == "CppCon");
    TEST_ENSURES(b.visit(string_visitor{}).empty());

    a = 2019;
    TEST_ENSURES(a.visit(int_visitor{}) == 2019);

    std::string s{"CppNow"};
    a = std::move(s);
    TEST_ENSURES(a.visit(string_visitor{}) == "CppNow");
  }

  { // index
    variant<int, std::string> v = "abc";

    TEST_ENSURES(v.index() == 1);
    v = {};
    TEST_ENSURES(v.index() == 0);
  }

  { // valueless_by_exception
    struct Demo {
      Demo(int) {}
      Demo(const Demo&) { throw std::domain_error("copy ctor"); }
      Demo& operator= (const Demo&) = default;
    };

    variant<std::string, Demo> var{"str"};
    TEST_ENSURES(var.index() == 0);
    TEST_ENSURES(std::get<0>(var) == "str");
    TEST_ENSURES(var.valueless_by_exception() == false);

    try {
      var = Demo{555};
      TEST_ENSURES(false);
    } catch (const std::domain_error& ex) {
      std::cout << "1) Exception: " << ex.what() << '\n';
    }
    TEST_ENSURES(var.index() == variant_npos);
    TEST_ENSURES(var.valueless_by_exception() == true);

    // Now the var is "valueless" which is an invalid state caused
    // by an exception raised in the process of type-changing assignment.

    try {
      std::get<1>(var);
      TEST_ENSURES(false);
    } catch (const bad_variant_access& ex) {
      std::cout << "2) Exception: " << ex.what() << '\n';
    }

    var = "str2";
    TEST_ENSURES(var.index() == 0);
    TEST_ENSURES(std::get<0>(var) == "str2");
    TEST_ENSURES(var.valueless_by_exception() == false);
  }

  { // emplace
    variant<std::string> v1;
    v1.emplace<0>("abc"); // OK
    TEST_ENSURES(std::get<0>(v1) == "abc");

    v1.emplace<std::string>("def"); // OK
    TEST_ENSURES(std::get<0>(v1) == "def");

    variant<std::string, std::string> v2;
    v2.emplace<1>("ghi"); // OK
    TEST_ENSURES(std::get<1>(v2) == "ghi");
    // v2.emplace<std::string>("abc");

  }

  {
    static_assert(std::is_constructible<variant<std::string>, decltype("abc")>::value, ""); // OK
    static_assert(std::is_constructible<variant<std::string, std::string>, decltype("abc")>::value == false, ""); // ill-formed
    static_assert(std::is_constructible<variant<std::string, const char*>, decltype("abc")>::value, ""); // OK, chooses const char*
    TEST_ENSURES((variant<std::string, const char*>("abc").index() == 1));
    TEST_ENSURES((variant<std::string, bool>("abc").index() == 0)); // OK, chooses string; bool is not a candidate
    TEST_ENSURES((variant<float, long, double>{0}.index() == 1)); // OK, holds long
  }

  { // variant_alternative
    using v = variant<int, float>;
    static_assert(std::is_same<int,   variant_alternative_t<0, v>>::value, "");
    static_assert(std::is_same<float, variant_alternative_t<1, v>>::value, "");
    // cv-qualification on the variant type propagates to the extracted alternative type.
    static_assert(std::is_same<const int, variant_alternative_t<0, const v>>::value, "");
  }

  { // swap
    variant<int, std::string> v1{2}, v2{"abc"};
    TEST_ENSURES(get<0>(v1) == 2);
    TEST_ENSURES(get<1>(v2) == "abc");
    v1.swap(v2);
    TEST_ENSURES(get<1>(v1) == "abc");
    TEST_ENSURES(get<0>(v2) == 2);
  }

  { // visit
    using var_t = variant<int, long, double, std::string>;

    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};

    for (auto& v: vec) {
        // 1. void visitor, only called for side-effects (here, for I/O)
        visit([](auto&& arg){ std::cout << arg; }, v);

        // 2. value-returning visitor, demonstrates the idiom of returning another variant
        var_t w = visit([](auto&& arg) -> var_t { return arg + arg; }, v);

        // 3. type-matching visitor: a lambda that handles each type differently
#if __cplusplus >= 201703L
        std::cout << ". After doubling, variant holds ";
        visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "int with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "long with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "double with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "std::string with value " << std::quoted(arg) << '\n';
            else
                static_assert(always_false<T>::value, "non-exhaustive visitor!");
        }, w);
#else
      std::cout << '\n';
#endif
    }

    std::stringstream ss;
    for (auto& v: vec) {
      // 4. another type-matching visitor: a class with 3 overloaded operator()'s
      // Note: The `(auto arg)` template operator() will bind to `int` and `long`
      //       in this case, but in its absence the `(double arg)` operator()
      //       *will also* bind to `int` and `long` because both are implicitly
      //       convertible to double. When using this form, care has to be taken
      //       that implicit conversions are handled correctly.
      visit(overloaded(
          [&](auto arg) { ss << arg << ' '; },
          [&](double arg) { ss << std::fixed << arg << ' '; },
          [&](const std::string& arg) { ss << std::quoted(arg) << ' '; }
      ), v);
    }
    TEST_ENSURES(ss.str() == R"(10 15 1.500000 "hello" )");
  }

  { // holds alternative
    variant<int, std::string> v = "abc";
    TEST_ENSURES(holds_alternative<int>(v) == false);
    TEST_ENSURES(holds_alternative<std::string>(v) == true);
  }

  { // get
    variant<int, float> v{12}, w;
    TEST_ENSURES(std::get<int>(v) == 12);
    w = std::get<int>(v);
    TEST_ENSURES(std::get<int>(w) == 12);
    w = std::get<0>(v); // same effect as the previous line
    TEST_ENSURES(std::get<0>(w) == 12);

    //  std::get<double>(v); // error: no double in [int, float]
    //  std::get<3>(v);      // error: valid index values are 0 and 1

    try {
      w = 42.0f;
      TEST_ENSURES(std::get<float>(w) == 42.0f);
      w = 42;
      std::cout << std::get<float>(w) << '\n'; // throws
      TEST_ENSURES(false);
    } catch (bad_variant_access const& ex) {
      std::cout << ex.what() << ": w contained int, not float\n";
    }
  }

  { // get_if
    variant<int, float> v{12}, w{3.f};
    TEST_ENSURES(get_if<int>(&v) != nullptr);
    TEST_ENSURES(std::get_if<int>(&w) == nullptr);
  }

  { // monostate
    struct S {
      S(int i) : i(i) {}
      int i;
    };

    variant<monostate, S> var;
    TEST_ENSURES(var.index() == 0);

    try {
      std::get<S>(var); // throws! We need to assign a value
      TEST_ENSURES(false);
    } catch(const bad_variant_access& e) {
      std::cout << e.what() << '\n';
    }

    var = 42;
    TEST_ENSURES(std::get<S>(var).i == 42);

    std::cout << "std::get: " << std::get<S>(var).i << '\n'
              << "std::hash: " << std::hex << std::showbase
              << std::hash<monostate>{}(monostate{}) << '\n';
  }

  {
    using v = variant<int, float, std::string>;
    v v1(1);
    v v2(2.0f);
    v v3("hello");

    v2.swap(v3);
    TEST_ENSURES(v2.index() == 2);
    TEST_ENSURES(v3.index() == 1);
  }

  { // comparison operators
    variant<int, std::string> v1, v2;

    { // operator==
      // by default v1 = 0, v2 = 0;
      TEST_ENSURES(v1 == v2);
      TEST_ENSURES(v1.index() == 0 && get<int>(v1) == 0);

      v1 = v2 = 1;
      TEST_ENSURES(v1 == v2);
      TEST_ENSURES(v1.index() == 0 && get<int>(v1) == 1);

      v2 = 2;
      TEST_ENSURES((v1 == v2) == false);
      TEST_ENSURES(v1.index() == 0 && get<int>(v1) == 1);
      TEST_ENSURES(v2.index() == 0 && get<int>(v2) == 2);

      v1 = "A";
      TEST_ENSURES((v1 == v2) == false);
      TEST_ENSURES(v1.index() == 1 && get<std::string>(v1) == "A");
      TEST_ENSURES(v2.index() == 0 && get<int>(v2) == 2);

      v2 = "B";
      TEST_ENSURES(v1 != v2);
      TEST_ENSURES(v1.index() == 1 && get<std::string>(v1) == "A");
      TEST_ENSURES(v2.index() == 1 && get<std::string>(v2) == "B");

      v2 = "A";
      TEST_ENSURES(v1 == v2);
      TEST_ENSURES(v1.index() == 1 && get<std::string>(v1) == "A");
      TEST_ENSURES(v2.index() == 1 && get<std::string>(v2) == "A");
    }

    { // operator<

      v1 = v2 = 1;
      TEST_ENSURES((v1 < v2) == false);

      v2 = 2;
      TEST_ENSURES(v1 < v2);

      v1 = 3;
      TEST_ENSURES((v1 < v2) == false);

      v1 = "A"; v2 = 1;
      TEST_ENSURES((v1 < v2) == false);
      TEST_ENSURES(v1.index() == 1 && v2.index() == 0);

      v1 = 1; v2 = "A";
      TEST_ENSURES(v1 < v2);
      TEST_ENSURES(v1.index() == 0 && v2.index() == 1);

      v1 = v2 = "A";
      TEST_ENSURES((v1 < v2) == false);

      v2 = "B";
      TEST_ENSURES(v1 < v2);

      v1 = "C";
      TEST_ENSURES((v1 < v2) == false);
    }

    {
      static_assert(equality_comparable_with<variant<int, std::string>, variant<int, std::string>>::value, "");
      static_assert(equality_comparable_with<variant<int, std::string>, variant<std::string, int>>::value == false, "");
    }
  }

  { // variant_size
    static_assert(variant_size<variant<>>::value == 0, "");
    static_assert(variant_size<variant<int>>::value == 1, "");
    static_assert(variant_size<variant<int, int>>::value == 2, "");
    static_assert(variant_size<variant<int, int, int>>::value == 3, "");
    static_assert(variant_size<variant<int, float, double>>::value == 3, "");
    static_assert(variant_size<variant<monostate, void>>::value == 2, "");
    static_assert(variant_size<variant<const int, const float>>::value == 2, "");
    // static_assert(variant_size<variant<variant<std::any>>>::value == 1);
  }

  { // hash
    using Var = variant<int, int, int, std::string>;

    Var var;
    std::get<0>(var) = 2020;
    std::cout << "get<0> = " << std::get<0>(var) << "\t" "# = " << std::hash<Var>{}(var) << '\n';

    var.emplace<1>(2023);
    std::cout << "get<1> = " << std::get<1>(var) << "\t" "# = " << std::hash<Var>{}(var) << '\n';

    var.emplace<2>(2026);
    std::cout << "get<2> = " << std::get<2>(var) << "\t" "# = " << std::hash<Var>{}(var) << '\n';

    var = "C++";
    std::cout << "get<3> = " << std::get<3>(var) << "\t" "# = " << std::hash<Var>{}(var) << '\n';
  }

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
