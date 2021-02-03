//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/math.hpp>


int main() {
  INIT_TEST("vccc::math")

  auto y = [](double x) { return x*x; };

  TEST_ENSURES(FLOAT_COMPARE((vccc::partialDiff<double, 0>(vccc::differential_five_point_stencil_t{}, y, std::make_tuple(1.))), 2., vccc::epsilon<double>()));


  return TEST_RETURN_RESULT;
}