//
// Created by YongGyu Lee on 2021/06/03.
//

#include <atomic>
#include <iostream>

#include "vccc/experimental/signal.hpp"
#include "vccc/test.hpp"

int main() {
  INIT_TEST("vccc::experimental::signal");

  {
    vccc::experimental::signal::signal<void()> signal;

    std::atomic_int sum{0};
    for(int i=0; i<100; ++i)
      signal.connect([&](){++sum;});
    signal();
    TEST_ENSURES(sum == 100);
  }

  return TEST_RETURN_RESULT;
}
