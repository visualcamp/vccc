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
#include "vccc/log.hpp"
#include "test_core.hpp"


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

  vccc::StreamWrapper stream;
  stream << vccc::Separator("");

  // test cout-like
  TEST_ENSURES(vccc::Logger{}.to_string(1) == "1");
  TEST_ENSURES(vccc::Logger{}.to_string(1,2) == "12");
  TEST_ENSURES(vccc::Logger{}.to_string(1,2,3) == "123");
  TEST_ENSURES(vccc::Logger{}.to_string("Hello,", "world!") == "Hello,world!");
  TEST_ENSURES(vccc::Logger{}.to_string("Hello,", std::string("world!")) == "Hello,world!");
  // TODO: ignore separator on manipulators
  TEST_ENSURES(vccc::Logger{}.to_string(vccc::Separator(" "), std::boolalpha, true, false, std::true_type{}, std::false_type{})
                == "true false true false");
  TEST_ENSURES(vccc::Logger{}.to_string(std::setprecision(5), 3.14159265) == "3.1416");
  TEST_ENSURES(vccc::Logger{}.to_string(foo{}) == message);

  // test manipulator
  {
    TEST_ENSURES(vccc::Logger{}.to_string(1,2,3) == "123");
    vccc::Logger::global_separator() = "?";
    TEST_ENSURES(vccc::Logger{}.to_string(1,2,3) == "1?2?3");
    vccc::Logger::global_separator().clear();
    TEST_ENSURES(vccc::Logger{}.to_string(1,2,3) == "123");

    TEST_ENSURES(vccc::Logger{}.to_string(vccc::Separator(" "),1,2,3) == "1 2 3");
    TEST_ENSURES(vccc::Logger{}.to_string(vccc::Separator("TONY"),1,2,3) == "1TONY2TONY3");
  }

  // test containers
  TEST_ENSURES(vccc::Logger{}.to_string(std::vector<int>{}) == "{}");
  TEST_ENSURES(vccc::Logger{}.to_string(std::vector<int>{1}) == "{ 1 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::vector<int>{1,2,3}) == "{ 1, 2, 3 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::vector<int>{1,2,3}, " ", std::vector<int>{4,5,6}) == "{ 1, 2, 3 } { 4, 5, 6 }");

  TEST_ENSURES(vccc::Logger{}.to_string(std::array<int, 3>{-1,-2,-3}) == "{ -1, -2, -3 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::list<int>{0,1,0}) == "{ 0, 1, 0 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::map<int, std::string>{{1,"one"},{2,"two"}})
                == "{ { 1: one }, { 2: two } }");
  TEST_ENSURES(vccc::Logger{}.to_string(bar{1,2,3}) == "{ 1, 2, 3 }");

  // test tuples
  TEST_ENSURES(vccc::Logger{}.to_string(std::pair<int, int>(1, 2)) == "{ 1, 2 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::make_tuple(1, "hi", std::vector<int>{0,0,0})) == "{ 1, hi, { 0, 0, 0 } }");

  // test integer_sequence
  TEST_ENSURES(vccc::Logger{}.to_string(std::make_index_sequence<3>{}) == "{ 0, 1, 2 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::index_sequence_for<int, float>{}) == "{ 0, 1 }");

  // test chronos
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::seconds(1)) == "1s");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::seconds(100)) == "1m 40s");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::nanoseconds(100)) == "100ns");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::nanoseconds(123'456)) == "123.46us");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::milliseconds(123)) == "123ms");
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
