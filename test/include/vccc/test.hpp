//
// Created by cosge on 2021-02-02.
//

#ifndef VCCC_TEST_CORE_HPP
#define VCCC_TEST_CORE_HPP

#include <sstream>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>

namespace test{
struct TestCounter {
  inline TestCounter(std::string test_name) : test_name(std::move(test_name)) {}
  inline bool testOne(bool expr, std::string location, std::string name="") {
    ++num_tests;
    if(expr) {
      ++num_passed;
    }
    else {
      addFail(std::move(location), std::move(name));
    }
    return expr;
  }
  inline void addFail(std::string location, std::string name="") {
    std::stringstream ss;
    ss << location << " " << name << "failed" << '\n';
    std::cerr << ss.str();
    ++num_failed;
    failed_locations.push_back({std::move(location), std::move(name)});
  }
  inline ~TestCounter() {
    std::stringstream ss1, ss2;
    ss2 << "(" << num_passed << '/'
        << num_tests  << ")";

    auto case_str = ss2.str();
    std::string cut_name;
    if(test_name.size() + case_str.size() > max_length)
      cut_name = test_name.substr(0,  max_length - 2 * case_str.size());
    else
      cut_name = test_name;

    ss1 << cut_name
        << std::string(max_length - (cut_name.size() + case_str.size()), spacer)
        << case_str;

    std::cout << ss1.str() << std::endl;

    if(num_failed > 0) {
      std::cerr << num_failed
                << (num_failed == 1 ? " test" : " tests")
                <<  " failed\n";
      for(const auto& location : failed_locations)
        std::cerr << location.test_name
                  << " failed at : "
                  << location.location
                  << '\n';
    }

  }
  int max_length = 80;
  char spacer = '.';

  std::string test_name;
  int num_tests = 0;
  int num_passed = 0;
  int num_failed = 0;
  struct FailData {
    std::string location;
    std::string test_name;
  };
  std::vector<FailData> failed_locations;

};
}

#define CALL_LOCATION             \
[](auto file, auto line) {        \
  std::stringstream ss;           \
  ss << file                      \
     << ", line "                 \
     << line;                     \
  return ss.str();                \
} (__FILE__, __LINE__)

#define DUMMY_NAME_IMPL(x) vccc_test_dummy ## _ ## x
#define DUMMY_NAME(x) DUMMY_NAME_IMPL(x)

#define TEST_GLOBAL_RESULT DUMMY_NAME(result)
#define TEST_RETURN_RESULT TEST_GLOBAL_RESULT ? 0 : 1

#define TEST_NAME DUMMY_NAME(name)
#define TEST_RESULT_CASE(index) DUMMY_NAME(__LINE__)
#define TEST_COUNTER_NAME DUMMY_NAME(counter)

#define INIT_TEST(test_name)                      \
bool TEST_GLOBAL_RESULT = true;                   \
const std::string TEST_NAME = (test_name);        \
auto TEST_COUNTER_NAME                            \
  = test::TestCounter(test_name);                 \

//#define TEST_ENSURES(expr)                      \
//do {                                            \
//  ++TEST_COUNTER_NAME.num_tests;                \
//  bool TEST_RESULT_CASE(__LINE__)               \
//    = (expr);                                   \
//  if (!TEST_RESULT_CASE(__LINE__)) {            \
//    ++TEST_COUNTER_NAME.num_failed;             \
//    TEST_COUNTER_NAME.addFail(CALL_LOCATION);   \
//  }                                             \
//  else {                                        \
//    ++TEST_COUNTER_NAME.num_passed;             \
//  }                                             \
//  TEST_GLOBAL_RESULT                            \
//    = TEST_GLOBAL_RESULT &&                     \
//      TEST_RESULT_CASE(__LINE__);               \
//} while(false)

#define TEST_ENSURES(expr)                        \
do {                                              \
  TEST_GLOBAL_RESULT                              \
    = TEST_COUNTER_NAME.testOne((expr),           \
                                CALL_LOCATION) && \
      TEST_GLOBAL_RESULT;                         \
} while(false)

#define FLOAT_COMPARE(x, y, e) (std::abs((x) - (y)) < (e))

namespace test{

struct CopyConstructable {};
struct CopyAssignable {};
struct MoveConstructable {};
struct MoveAssignable {};

struct Movable : public MoveConstructable, public MoveAssignable {};
struct Copyable : public CopyConstructable, public CopyAssignable {};

struct NonCopyConstructable {
  NonCopyConstructable(const NonCopyConstructable&) = delete;
  NonCopyConstructable(NonCopyConstructable&&) {};

  NonCopyConstructable& operator = (const NonCopyConstructable&) {return *this;};
  NonCopyConstructable& operator = (NonCopyConstructable&) {return *this;};
};

struct NonCopyAssignable {
  NonCopyAssignable(const NonCopyAssignable&) {};
  NonCopyAssignable(NonCopyAssignable&&) {};

  NonCopyAssignable& operator = (const NonCopyAssignable&) = delete;
  NonCopyAssignable& operator = (NonCopyAssignable&) {return *this;};
};

struct NonMoveConstructable {
  NonMoveConstructable(const NonMoveConstructable&) {};
  NonMoveConstructable(NonMoveConstructable&&) = delete;

  NonMoveConstructable& operator = (const NonMoveConstructable&) {return *this;}
  NonMoveConstructable& operator = (NonMoveConstructable&) {return *this;};
};

struct NonMoveAssignable {
  NonMoveAssignable(const NonMoveAssignable&) {};
  NonMoveAssignable(NonMoveAssignable&&) {};

  NonMoveAssignable& operator = (const NonMoveAssignable&) {return *this;};
  NonMoveAssignable& operator = (NonMoveAssignable&) = delete;
};



}

#endif //VCCC_TEST_CORE_HPP
