//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/numeric.hpp>
#include <array>
#include <vector>

struct tt {
  int x;
};
int main() {
  INIT_TEST("vccc::numeric")

  std::vector<int> v0;
  std::vector<int> v1 = {1,2,3};

  std::vector<tt> v2 = {{1},{2},{3}};

  std::cout << vccc::sum(v0.begin(), v0.end()) << std::endl;
  std::cout << vccc::sum(v1.begin(), v1.end()) << std::endl;
  std::cout << vccc::average(v0.begin(), v0.end()) << std::endl;
  std::cout << vccc::average(v1.begin(), v1.end()) << std::endl;
  std::cout << vccc::average(&(*v1.begin()), &(*v1.end())) << std::endl;
  std::cout << vccc::average(v2.begin(), v2.end(), [](const auto& t){ return t.x;}) << std::endl;

  return TEST_RETURN_RESULT;
}