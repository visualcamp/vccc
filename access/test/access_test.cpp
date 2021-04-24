//
// Created by cosge on 2021-04-24.
//

#include <iostream>

#include "vccc/access.hpp"
#include "vccc/test.hpp"

struct foo {
 private:
  int x = 10;
 public:
  int y = -10;
};

using foo_x_tag = vccc::AccessTag<class foo_x, foo>;
template struct vccc::Accessor<foo_x_tag, decltype(foo::x), &foo::x>;

int main() {
  INIT_TEST("access_test")

  foo f;
  constexpr int foo::* ptr = (int foo::*)(&foo::y);

//  std::cout << vccc::access<foo_x_tag, int, ptr>(f, foo_x_tag()) << std::endl;
//  std::cout << access<foo_x_tag>(f) << std::endl;

  vccc::matching_accessor_t<foo_x_tag> ee;
  std::cout << f.*vccc::matching_accessor_t<foo_x_tag>::ptr << std::endl;
  std::cout << vccc::access<foo_x_tag>(f) << std::endl;

  std::cout << std::endl;
  std::cout << "etete";

  return TEST_RETURN_RESULT;
}