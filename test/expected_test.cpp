//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "vccc/expected.hpp"
#include "vccc/string_view.hpp"
#include "vccc/type_traits.hpp"
#include "test_core.hpp"

namespace vccc {
namespace {


int Test() {
  INIT_TEST("vccc::expected")

  struct foo {
    foo(int x) noexcept : x(x) {
      std::cout << x << " constructed." << std::endl;
    }
    foo(const foo& f) : x(f.x) {
      std::cout << x << " copy constructed" << std::endl;
    }
    foo(foo&& f) : x(f.x) {
      if (f.x == 1)
        throw 1;
      std::cout << x << " move constructed" << std::endl;
    }
    foo& operator=(const foo& f) {
      x = f.x;
      std::cout << x << " copy assigned" << std::endl;
      return *this;
    }
    foo& operator=(foo&& f) {
      x = f.x;
      std::cout << x << " move assigned" << std::endl;
      return *this;
    }
    ~foo() {
      std::cout << x << " destroyed." << std::endl;
    }
    int x;
  };

  {
    expected<void, int> e;
    e.and_then([]() { return expected<void, int>(); });
    e.transform([]() { /* no-op */ });
    e.or_else([](int) { return expected<void, double>{}; });
    e.transform_error([](int) { return ""; });
    e.emplace();
  }

  {
    expected<foo, std::string> e = 1;
    e.and_then([](const foo&) { return expected<int, std::string>(); });
    e.transform([](const foo&) { return expected<void, std::string>(); });
    e.or_else([](const std::string&) { return expected<foo, std::string>{1}; });
    e.transform_error([](const std::string&) { return 123123; });
    e.emplace(2);
  }

  {
    enum class parse_error {
      invalid_input,
      overflow
    };

    auto parse_number = [](vccc::string_view str) -> vccc::expected<double, parse_error> {
      const char* begin = str.data();
      char* end;
      double retval = std::strtod(begin, &end);

      if (begin == end)
        return vccc::unexpected<parse_error>(parse_error::invalid_input);
      else if (std::isinf(retval))
        return vccc::unexpected<parse_error>(parse_error::overflow);

      str.remove_prefix(end - begin);
      return retval;
    };

    TEST_ENSURES(*parse_number("42") == 42);
    TEST_ENSURES(parse_number("42abc") == 42);
    TEST_ENSURES(parse_number("meow").error() == parse_error::invalid_input);
    TEST_ENSURES(parse_number("inf").error() == parse_error::overflow);
  }

  expected<foo, foo> e1 = 1;
  expected<foo, foo> e2{unexpected<foo>{foo{2}}};

  std::cout << "Start" << std::endl;
  try {
    e2 = std::move(e1);
  } catch (...) {
    std::cout << std::boolalpha << e2.has_value() << std::endl;
  }
  std::cout << "End" << std::endl;


  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
