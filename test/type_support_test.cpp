//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include "vccc/type_support.hpp"
#include <array>
#include <vector>


int main() {
  INIT_TEST("vccc::type_support")

  std::vector<double> v = {1.3,2.6,3.9};

# if VCCC_USE_OPENCV_FEATURES
  TEST_ENSURES(vccc::vtype_convert<int>(v)[1] == 3);

  cv::Point2f p(1.3,2.6);
  auto vec = vccc::convert_to<cv::Vec2i>(p);
  TEST_ENSURES(vccc::at<1>(vec) == 3);
  auto vec2 = vccc::convert_to<std::vector<int>>(p);
  TEST_ENSURES((vec2.size() == 2));
  TEST_ENSURES((vec2[1] == 3));
# else
  TEST_ENSURES(vccc::vtype_convert<int>(v)[1] == 2);
# endif



  return TEST_RETURN_RESULT;
}
