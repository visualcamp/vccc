//
// Created by YongGyu Lee on 2021/06/03.
//

#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

#include "vccc/experimental/signal.hpp"
#include "vccc/test.hpp"
#include "vccc/log.hpp"

using namespace std::chrono_literals;

int main() {
  INIT_TEST("vccc::experimental::signal")

  using clock = std::chrono::steady_clock;
  auto& now = clock::now;
  std::uniform_int_distribution<int> dist(0, 3);
  std::mt19937 gen{std::random_device{}()};

  { // call & disconnect thread-safety test
    vccc::experimental::signal::signal<void()> signal;
    std::atomic_int called{0};
    int test_count = 100000;
    for(int i=0; i<test_count; ++i) {
      vccc::experimental::signal::connection conn = signal.connect([&]{++called;});
      std::thread t2([&] { for(int i=0; i<dist(gen); ++i) std::this_thread::yield(); signal(); });
      std::thread t1([&] { conn.disconnect(); });
      std::thread t3([&] { conn.disconnect(); });
      std::thread t4([&] { conn.disconnect(); });
      t1.join();
      t2.join();
      t3.join();
      t4.join();
    }
    LOGE("called:", called, "/", 100000);
  }

  { // basic call operation test
    vccc::experimental::signal::signal<void()> signal;

    TEST_ENSURES((signal(), true));

    std::atomic_int sum{0};
    int test_count = 100000;

    auto t1 = now();
    for(int i=0; i<test_count; ++i)
      signal.connect([&](){++sum;});
    auto t2 = now();

    TEST_ENSURES(signal.size() == test_count);

    LOGW("Connecting", signal.size(), "slots took", t2 - t1);

    t1 = now();
    signal();
    t2 = now();
    TEST_ENSURES(sum == test_count);
    LOGW("Invoking", signal.size(), "slots took", t2 - t1);
  }

  {

  }


  return TEST_RETURN_RESULT;
}
