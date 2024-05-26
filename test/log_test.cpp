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
  TEST_ENSURES(vccc::Logger{}.to_string(vccc::Quoted{}, std::map<int, std::string>{{1,"one"},{2,"two"}})
                == "{ 1 => \"one\", 2 => \"two\" }");
  TEST_ENSURES(vccc::Logger{}.to_string(bar{1,2,3}) == "{ 1, 2, 3 }");

  // test tuples
  TEST_ENSURES(vccc::Logger{}.to_string(std::pair<int, int>(1, 2)) == "{ 1, 2 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::make_tuple(1, "hi", std::vector<int>{0,0,0})) == "{ 1, hi, { 0, 0, 0 } }");

  // test integer_sequence
  TEST_ENSURES(vccc::Logger{}.to_string(std::make_index_sequence<3>{}) == "{ 0, 1, 2 }");
  TEST_ENSURES(vccc::Logger{}.to_string(std::index_sequence_for<int, float>{}) == "{ 0, 1 }");

  // test chronos
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::seconds(1)) == "1s");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::nanoseconds(100)) == "100ns");
#if __cplusplus < 202002L
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::seconds(100)) == "1m 40s");
  TEST_ENSURES(vccc::Logger{}.to_string(std::chrono::nanoseconds(123'456)) == "123.46us");
#endif
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

  auto address_to_string = [](const void* p) {
    std::stringstream ss;
    ss << p;
    return ss.str();
  };

  // test non-printable
  struct bar {};
  bar b;
  TEST_ENSURES(vccc::Logger{}.to_string(b) == '@' + address_to_string(std::addressof(b)));

  LOGD("THIS ", "IS ", "DEBUG");
  LOGI("THIS ", "IS ", "INFO");
  LOGW("THIS ", "IS ", "WARN");
  LOGE("THIS ", "IS ", "ERROR");

  vccc::Log(vccc::Logger::kError, "my tag ", "hello, ", 'w', 0, "rld", "!");

  // test non-empty aggregate types

  struct agg {
    int x;
    std::string s;
  };
  agg a{1, "foo"};
#if __cplusplus < 201703L
  TEST_ENSURES(vccc::Logger{}.to_string(a) == '@' + address_to_string(std::addressof(a)));
#else
  TEST_ENSURES(vccc::Logger{}.to_string(vccc::ExpandAggregate{}, a) == "{ 1, foo }");
#endif

  vccc::Logger l;
  using complex_type = std::map<std::string, std::pair<agg, bar>>;
  complex_type c = {
      {"first", {{1, "one"}, {}}},
      {"second", {{2, "two"}, {}}}
  };

  auto str = l.to_string(vccc::Quoted{}, vccc::ExpandAggregate{}, c);

#if __cplusplus < 201703L
  TEST_ENSURES(str ==
      "{ "
          "\"first\""  " => { " "@" + address_to_string(std::addressof(c["first" ].first)) + ", " "@" + address_to_string(std::addressof(c["first" ].second)) + " }"  ", "
          "\"second\"" " => { " "@" + address_to_string(std::addressof(c["second"].first)) + ", " "@" + address_to_string(std::addressof(c["second"].second)) + " }"
      " }");
#else
  TEST_ENSURES(str ==
      "{ "
          "\"first\""  " => { { 1, \"one\" }, " "@" + address_to_string(std::addressof(c["first" ].second)) + " }"  ", "
          "\"second\"" " => { { 2, \"two\" }, " "@" + address_to_string(std::addressof(c["second"].second)) + " }"
      " }");
#endif

  int arr[] = {1,2,3,4,5};
  TEST_ENSURES(l.to_string(arr) == address_to_string(std::addressof(arr)));
  TEST_ENSURES(l.to_string(vccc::ExpandArray{}, arr) == "{ 1, 2, 3, 4, 5 }");

  return TEST_RETURN_RESULT;
}
