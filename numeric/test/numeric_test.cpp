//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/numeric.hpp>
#include <array>

int main() {
  INIT_TEST("vccc::numeric")

  std::vector<const int> v0;
  std::vector<const int> v1 = {1,2,3};

  std::cout << vccc::sum(v0.begin(), v0.end()) << std::endl;
  std::cout << vccc::sum(v1.begin(), v1.end()) << std::endl;
  std::cout << vccc::average(v0.begin(), v0.end()) << std::endl;
  std::cout << vccc::average(v1.begin(), v1.end()) << std::endl;
  std::cout << vccc::average(&(*v1.begin()), &(*v1.end())) << std::endl;

  return TEST_RETURN_RESULT;
}