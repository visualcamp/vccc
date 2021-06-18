//
// Created by YongGyu Lee on 2021/06/03.
//

#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

#include "vccc/experimental/signal.hpp"
#include "vccc/test.hpp"
#include "vccc/log.hpp"

using namespace std::chrono_literals;
namespace signal2 = vccc::experimental;

int main() {
  INIT_TEST("vccc::experimental::signal")

  using clock = std::chrono::steady_clock;
  auto& now = clock::now;
  std::uniform_int_distribution<int> dist(0, 3);
  std::mt19937 gen{std::random_device{}()};
  auto random_int = [](int a, int b) {
    std::uniform_int_distribution<int> dist(a, b);
    std::mt19937 gen{std::random_device{}()};
    return dist(gen);
  };

  { // basic call operation test
    vccc::experimental::signal<void()> signal;

    TEST_ENSURES((signal(), true));

    int sum = 0;
    int test_count = 100000;

    auto t1 = now();
    for(int i=0; i<test_count; ++i)
      signal.connect([&](){++sum;});
    auto t2 = now();

    TEST_ENSURES(signal.size() == test_count);

    LOGI("Connecting", signal.size(), "slots took", t2 - t1);

    t1 = now();
    signal();
    t2 = now();
    TEST_ENSURES(sum == test_count);
    LOGI("Invoking", signal.size(), "slots took", t2 - t1);
  }

  { // object safety test
    signal2::connection conn;
    std::atomic_int dummy{0};
    int test_count = 100000;

    for (int i=0; i<test_count; ++i) {
      std::thread t;
      {
        signal2::signal<void()> sig;
        conn = sig.connect([&] { ++dummy; });
        t = std::thread([&]{
          for(int i=0; i<dist(gen)*10; ++i) std::this_thread::yield();
          conn.disconnect();
        });
      }
      t.join();
    }
  }

  { // connect order test
    signal2::signal<int()> sig;
    sig.connect([]{return 1;});
    TEST_ENSURES(*sig() == 1);

    sig.connect([]{return 2;}, signal2::at_front);
    TEST_ENSURES(*sig() == 1);

    sig.disconnect(signal2::ungrouped_back);
    TEST_ENSURES(sig.size() == 1);

    sig.connect(1, []{return 3;});
    TEST_ENSURES(*sig() == 3);

    sig.connect(2, []{return 4;});
    TEST_ENSURES(*sig() == 4);

    sig.connect(0, []{return 5;});
    TEST_ENSURES(*sig() == 4);

    sig.disconnect(2);
    TEST_ENSURES(sig.size() == 3);
  }

  { // track test(sync)
    signal2::signal<int()> sig;
    int called = 0;
    auto ptr = std::make_shared<int>(1);
    auto c1 = sig.connect([&]{ return ++called; }).track(ptr);
    TEST_ENSURES(called == 0);
    sig();
    TEST_ENSURES(called == 1);
    ptr.reset();
    sig();
    TEST_ENSURES(called == 1);
    ptr = std::make_shared<int>(1);
    sig();
    TEST_ENSURES(called == 1);
    c1 = sig.connect([&]{ return ++called; }).track(ptr);
    sig();
    TEST_ENSURES(called == 2);
  }

  { // track test(async)
    auto t = now();
    signal2::signal<void()> sig;
    std::atomic_int called{0};
    int test_count = 100000;
    for (int i=0; i<test_count; ++i) {
      auto ptr = std::make_shared<std::vector<int>>(3);
      auto conn = sig.connect([&, ptr = ptr.get()](){
        ++called;
        auto s = ptr->size();
      }).track(ptr);
      std::thread t1([&]{for(int j=0;j<dist(gen);++j) std::this_thread::yield(); ptr.reset();});
      std::thread t2([&]{sig();});
      t1.join();
      t2.join();
      conn.disconnect();
    }

    LOGI("Track test:", now() - t, "test count", test_count, "called", called);
  }

//  { // disconnect sync test
//    signal2::signal<void()> sig;
//    std::atomic_int num{0};
//    auto conn = sig.connect([&]{
//      std::this_thread::sleep_for(10s);
//      int expected = 0;
//      num.compare_exchange_strong(expected, 99);
//    });
//    std::thread t1([&]{sig();});
//    std::this_thread::sleep_for(1s);
//    conn.disconnect();
//    num.store(1);
//    t1.join();
//    TEST_ENSURES(num == 1);
//  }

  { // call & disconnect thread-safety test
    auto t = now();
    vccc::experimental::signal<void()> signal;
    std::atomic_int called{0};
    int test_count = 100000;
    for(int i=0; i<test_count; ++i) {
      vccc::experimental::connection conn = signal.connect([&]{++called;});
      std::thread t2([&] { for(int i=0; i<dist(gen); ++i) std::this_thread::yield(); signal(); });
      std::thread t1([&] { conn.disconnect(); });
      std::thread t3([&] { conn.disconnect(); });
      std::thread t4([&] { conn.disconnect(); });
      t1.join();
      t2.join();
      t3.join();
      t4.join();
    }
    LOGI("Thread safety test:", now() - t, "called", called, "/", test_count);
  }

  { // call & connect & disconnect thread-safety test
    auto t = now();
    vccc::experimental::signal<void()> signal;
    std::atomic_int called{0};
    std::atomic_int disconnected{0};
    std::atomic_int connected{0};
    std::mutex conn_m;
    std::shared_ptr<signal2::connection> conn;
    int test_count = 100000;
    auto random_work = [&]() -> std::function<void()> {
      switch(random_int(0, 10)) {
        case 0:
        case 1:
          return [&]{ // call
            for(int i=0; i<dist(gen); ++i) std::this_thread::yield();
            ++called;
          };
        case 2:
        case 3:
          return [&]{ // connect
            for(int i=1; i<dist(gen); ++i) std::this_thread::yield();
            {
              std::lock_guard<std::mutex> lck(conn_m);
              conn = std::make_shared<signal2::connection>(signal.connect([&]{++called;}));
            }
            ++connected;
          };
        default:
          return [&]{ // disconnect
            for(int i=2; i<dist(gen); ++i) std::this_thread::yield();
            std::weak_ptr<signal2::connection> wptr;
            {
              std::lock_guard<std::mutex> lck(conn_m);
              wptr = conn;
            }
            auto copy = wptr.lock();
            if(copy!=nullptr && copy->is_connected()) {
              copy->disconnect();
              ++disconnected;
            }
          };
      }
    };

    for(int i=0; i<test_count; ++i) {
      std::thread t1(random_work());
      std::thread t2(random_work());
      std::thread t3(random_work());
      t1.join();
      t2.join();
      t3.join();
    }
    LOGI("Thread safety test2:", now() - t,
         "test count", test_count,
         "called", called,
         "connected", connected,
         "disconnected", disconnected,
         "remained connections", signal.size());
//    TEST_ENSURES(called <= connected);
    TEST_ENSURES(connected - disconnected <= signal.size());
  }

  { // return test
    vccc::experimental::signal<int()> sig;
    auto res = sig();
    TEST_ENSURES(res.has_value() == false);

    sig.connect([]{return 3;});
    res = sig();
    TEST_ENSURES(*res == 3);

    sig.connect([]{return 4;});
    TEST_ENSURES(*sig() == 4);
  }


  return TEST_RETURN_RESULT;
}
