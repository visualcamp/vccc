//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <thread>
#include <list>
#include <map>
#include <tuple>
#include <utility>
#include <vccc/log.hpp>
#include <vccc/test.hpp>


struct foo {};
const char message[] = "Custom Operator Overload";

std::ostream& operator << (std::ostream& os, const foo&) {
  os << message;
  return os;
}


struct bar {
  bar(int n) : data(n) {}
  bar(std::initializer_list<int> il) : data(il) {}
  std::size_t size() const { return data.size(); }
  auto begin() const { return data.cbegin(); }
  auto end() const { return data.cend(); }

  std::vector<int> data;
};

int main() {
  INIT_TEST("vccc::log")

  // test printf-like
  TEST_ENSURES(vccc::Logger("").get() == "");
  TEST_ENSURES(vccc::Logger("Hello, world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("Hello, %s", "world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("%d %.2f", 1234567, 123.4567).get() == "1234567 123.46");
  TEST_ENSURES(vccc::Logger("%p", NULL).get().size() != 0);
  TEST_ENSURES(vccc::Logger("%s", message).get() == message);

  // test cout-like
  TEST_ENSURES(vccc::Logger(1).get() == "1");
  TEST_ENSURES(vccc::Logger(1,2).get() == "1 2");
  TEST_ENSURES(vccc::Logger(1,2,3).get() == "1 2 3");
  TEST_ENSURES(vccc::Logger("Hello,", "world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("Hello,", std::string("world!")).get() == "Hello, world!");
  // TODO: ignore separator on manipulators
  TEST_ENSURES(vccc::Logger(std::boolalpha, true, false, std::true_type{}, std::false_type{}).get()
                == " true false true false");
  TEST_ENSURES(vccc::Logger(std::setprecision(5), 3.14159265).get() == " 3.1416");
  TEST_ENSURES(vccc::Logger(foo{}).get() == message);


  // test containers
  TEST_ENSURES(vccc::Logger(std::vector<int>{}).get() == "{}");
  TEST_ENSURES(vccc::Logger(std::vector<int>{1}).get() == "{ 1 }");
  TEST_ENSURES(vccc::Logger(std::vector<int>{1,2,3}).get() == "{ 1, 2, 3 }");
  TEST_ENSURES(vccc::Logger(std::vector<int>{1,2,3}, std::vector<int>{4,5,6}).get() == "{ 1, 2, 3 } { 4, 5, 6 }");

  TEST_ENSURES(vccc::Logger(std::array<int, 3>{-1,-2,-3}).get() == "{ -1, -2, -3 }");
  TEST_ENSURES(vccc::Logger(std::list<int>{0,1,0}).get() == "{ 0, 1, 0 }");
  TEST_ENSURES(vccc::Logger(std::map<int, std::string>{{1,"one"},{2,"two"}}).get()
                == "{ { 1: one }, { 2: two } }");
  TEST_ENSURES(vccc::Logger(bar{1,2,3}).get() == "{ 1, 2, 3 }");

  // test tuples
  TEST_ENSURES(vccc::Logger(std::pair<int, int>(1, 2)).get() == "{ 1, 2 }");
  TEST_ENSURES(vccc::Logger(std::make_tuple(1, "hi", std::vector<int>{0,0,0})).get() == "{ 1, hi, { 0, 0, 0 } }");

  // test integer_sequence
  TEST_ENSURES(vccc::Logger(std::make_index_sequence<3>{}).get() == "{ 0, 1, 2 }");
  TEST_ENSURES(vccc::Logger(std::index_sequence_for<int, float>{}).get() == "{ 0, 1 }");

  // test chronos
  TEST_ENSURES(vccc::Logger(std::chrono::seconds(1)).get() == "1s");
  TEST_ENSURES(vccc::Logger(std::chrono::seconds(100)).get() == "1m 40s");
  TEST_ENSURES(vccc::Logger(std::chrono::nanoseconds(100)).get() == "100ns");
  TEST_ENSURES(vccc::Logger(std::chrono::nanoseconds(123'456)).get() == "123.46us");
  TEST_ENSURES(vccc::Logger(std::chrono::milliseconds(123)).get() == "123ms");
  LOGD(std::chrono::seconds(1));
  LOGD(std::chrono::seconds(100));
  LOGD(std::chrono::minutes(1));
  LOGD(std::chrono::minutes(10));
  LOGD(std::chrono::minutes(60));
  LOGD(std::chrono::hours(24));
  LOGD(std::chrono::nanoseconds(123));
  LOGD(std::chrono::nanoseconds(123'4));
  LOGD(std::chrono::nanoseconds(123'45));
  LOGD(std::chrono::nanoseconds(123'456));
  LOGD(std::chrono::nanoseconds(123'456'789));
  LOGD(std::chrono::milliseconds(123));
  LOGD(std::chrono::time_point<std::chrono::system_clock>(std::chrono::hours(24)*365*1));

  LOGD("THIS", "IS", "DEBUG");
  LOGI("THIS", "IS", "INFO");
  LOGW("THIS", "IS", "WARN");
  LOGE("THIS", "IS", "ERROR");

//  vccc::StreamWrapper<std::stringstream> sw;
//
//  sw << std::vector<std::vector<int>>{{1, 2, 3}, {}};
//  LOGD(sw.stream().str()); sw.stream().str("");
//  sw << std::map<std::string, std::string>{{"key", "value"}};
//  LOGD(sw.stream().str()); sw.stream().str("");
//  sw << std::make_tuple();
//  LOGD(sw.stream().str()); sw.stream().str("");
//  sw << std::make_index_sequence<0>();
//  LOGD(sw.stream().str()); sw.stream().str("");
//  sw << std::make_index_sequence<3>();
//  LOGD(sw.stream().str()); sw.stream().str("");
//
//  sw << std::chrono::system_clock::now();
//  LOGD(sw.stream().str()); sw.stream().str("");
//
//  auto now = std::chrono::steady_clock::now();
//  for(int i=0; i<3600*3; ++i) {
//    sw << now;
//    std::cout << sw.stream().str() << '\n';
//    sw.stream().str("");
//    now += std::chrono::milliseconds (7007);
//  }
//
//  vccc::IOSFlagsSaver<std::ostream> saver(std::cout);


  return TEST_RETURN_RESULT;
}