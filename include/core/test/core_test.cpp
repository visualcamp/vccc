//
// Created by YongGyu Lee on 2023/12/27.
//

#include "test_core.hpp"
#include <cmath>
#include <array>

#include "vccc/core.hpp"
extern int bar(const char *a, int b);
void foo(int x);
#define BAR(a, b) ({ static_assert((b), "failed"); bar((a), (b));})


namespace {

int Test() {
  INIT_TEST("vccc::core")

  vccc::byte b{40};
  (void) b;

  static_assert(std::is_trivially_default_constructible<vccc::byte>::value, "");
  static_assert(std::is_trivially_destructible<vccc::byte>::value, "");
  static_assert(std::is_trivially_copy_constructible<vccc::byte>::value, "");
  static_assert(std::is_trivially_copy_assignable<vccc::byte>::value, "");
  static_assert(std::is_trivially_move_constructible<vccc::byte>::value, "");
  static_assert(std::is_trivially_move_assignable<vccc::byte>::value, "");

  static_assert(std::is_trivially_constructible<vccc::byte, unsigned char>::value == false, "");
  static_assert(std::is_constructible<vccc::byte, unsigned char>::value == false, "");
  static_assert(std::is_convertible<vccc::byte, unsigned char>::value == false, "");


  // Example got from https://en.cppreference.com/w/cpp/language/enum#enum_relaxed_init_cpp17

  struct A { vccc::byte b; };
  // A a1 = {{42}};     // error (copy-list-initialization of a constructor parameter)
  A a2 = {vccc::byte{42}}; // OK

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
