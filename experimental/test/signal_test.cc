//
// Created by YongGyu Lee on 2021/06/03.
//

#include <iostream>

#include "vccc/experimental/signal.hpp"
#include "vccc/test.hpp"

int main() {
  INIT_TEST("vccc::experimental::signal");

  vccc::experimental::signal::signal<void()> signal;

  return TEST_RETURN_RESULT;
}
