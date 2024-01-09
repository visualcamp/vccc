//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"

#include <cmath>
#include <array>
#include <vector>

#include "vccc/numeric.hpp"

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
//  std::cout << vccc::average(v0.begin(), v0.end()) << std::endl;  // Assert
  std::cout << vccc::average(v1.begin(), v1.end()) << std::endl;
  std::cout << vccc::average(v2.begin(), v2.end(), [](const auto& t){ return t.x;}) << std::endl;


  // Compare same numbers
  TEST_ENSURES(vccc::float_equal(0, 0) == true);
  TEST_ENSURES(vccc::float_equal(1, 1) == true);
  TEST_ENSURES(vccc::float_equal(0.0f, 0.0f) == true);
  TEST_ENSURES(vccc::float_equal(1E-300, 1E-300) == true);

  // Compare different numbers
  TEST_ENSURES(vccc::float_equal(1.0, 2.0) == false);
  TEST_ENSURES(vccc::float_equal(1.0, 2.0f) == false);
  TEST_ENSURES(vccc::float_equal(1.0f, 2.0) == false);
  TEST_ENSURES(vccc::float_equal(0, 0) == true);
  TEST_ENSURES(vccc::float_equal(0, 1) == false);
  TEST_ENSURES(vccc::float_equal(0, 1E-30f) == true);
  TEST_ENSURES(vccc::float_equal(0, 1E+30f) == false);
  TEST_ENSURES(vccc::float_equal(1E-30f, 1E+30f) == false);
  TEST_ENSURES(vccc::float_equal(0, 1E-300) == true);
  TEST_ENSURES(vccc::float_equal(0, 1E+300) == false);
  TEST_ENSURES(vccc::float_equal(1E-300, 1E+300) == false);

  // Compare negative numbers
  TEST_ENSURES(vccc::float_equal(-0.0, 0.0) == true);
  TEST_ENSURES(vccc::float_equal(-1, 1) == false);
  TEST_ENSURES(vccc::float_equal(-1E-30f, 0) == true);
  TEST_ENSURES(vccc::float_equal(-1E-30f, 1E-30f) == true);
  TEST_ENSURES(vccc::float_equal(-1E-300, 1E-300) == true);

  // Compare similar numbers
  TEST_ENSURES(vccc::float_equal(1.00000f, 1.00001f) == true);
  TEST_ENSURES(vccc::float_equal(1.00000, 1.00001) == false);
  TEST_ENSURES(vccc::float_equal(12345.0, std::nextafter(12345.0, 100000.0)) == true);
  TEST_ENSURES(vccc::float_equal(1.00000f, 1.00001) == false);
  TEST_ENSURES(vccc::float_equal(1.00000, 1.00001f) == false);

  // Compare small numbers
  TEST_ENSURES(vccc::float_equal(1E-10f, 0.5E-10f) == true);
  TEST_ENSURES(vccc::float_equal(0, vccc::epsilon<double>()) == true);
  TEST_ENSURES(vccc::float_equal(0, vccc::epsilon<double>() * 2) == false);
  TEST_ENSURES(vccc::float_equal(100, 100 + vccc::epsilon<double>()) == true);
  TEST_ENSURES(vccc::float_equal(1.0, 1.0 + vccc::epsilon<double>()) == true);
  TEST_ENSURES(vccc::float_equal(1.0, vccc::square(1.0 + vccc::epsilon<double>())) == false);

  // Compare big float
  TEST_ENSURES(1E+30f == 1E+30f + 1E+1);
  TEST_ENSURES(std::abs(1E+30f - std::nextafter(1E+30f, 1E+31f)) > vccc::epsilon<float>());
  TEST_ENSURES(vccc::float_equal(1E+30f, std::nextafter(1E+30f, 1E+31f)) == true);
  TEST_ENSURES(vccc::float_equal(1E+30f, 1E+30f * (1 + vccc::epsilon<float>())) == true);
  TEST_ENSURES(vccc::float_equal(1E+30f, 1E+30f * (1 + vccc::epsilon<float>() * 2)) == false);
  TEST_ENSURES(vccc::float_equal(1E+30f, 1E+30f * vccc::square(1 + vccc::epsilon<float>())) == false);

  // Compare big double
  TEST_ENSURES(1E+300 == 1E+300 + 1E+10);
  TEST_ENSURES(std::abs(1E+300 - std::nextafter(1E+300, 1E+301)) > vccc::epsilon<double>());
  TEST_ENSURES(vccc::float_equal(1E+300, std::nextafter(1E+300, 1E+301)) == true);
  TEST_ENSURES(vccc::float_equal(1E+300, 1E+300 * (1 + vccc::epsilon<double>())) == true);
  TEST_ENSURES(vccc::float_equal(1E+300, 1E+300 * (1 + vccc::epsilon<double>() * 2)) == false);
  TEST_ENSURES(vccc::float_equal(1E+300, 1E+300 * vccc::square(1 + vccc::epsilon<double>())) == false);

  TEST_ENSURES(vccc::float_equal_to<float>{}(1.0f, 1.00000001f) == true);
  TEST_ENSURES(vccc::float_equal_to<>{}(1.0f, 1.00000001f) == true);
  TEST_ENSURES(vccc::float_equal_to<>{}(1, 1.00000001f) == true);

  return TEST_RETURN_RESULT;
}
