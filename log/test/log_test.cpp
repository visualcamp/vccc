//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <array>
#include <vccc/log.hpp>
#include <vector>
#include <thread>

int main() {
  INIT_TEST("vccc::log")

  TEST_ENSURES(vccc::Logger("").get() == "");
  TEST_ENSURES(vccc::Logger("Hello, world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("Hello,", "world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("Hello, %s", "world!").get() == "Hello, world!");
  TEST_ENSURES(vccc::Logger("Hello,", std::string("world!")).get() == "Hello, world!");

  TEST_ENSURES(vccc::Logger(1).get() == "1");
  TEST_ENSURES(vccc::Logger(1,2).get() == "1 2");
  TEST_ENSURES(vccc::Logger(1,2,3).get() == "1 2 3");

  TEST_ENSURES(vccc::Logger(std::vector<int>{}).get() == "{}");
  TEST_ENSURES(vccc::Logger(std::vector<int>{1}).get() == "{ 1 }");
  TEST_ENSURES(vccc::Logger(std::vector<int>{1,2,3}).get() == "{ 1, 2, 3 }");


  return TEST_RETURN_RESULT;
}