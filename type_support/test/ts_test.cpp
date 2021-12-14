//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/type_support.hpp>
#include <array>
#include <vector>

#include <string>
#include <tuple>
#include <utility>
#include <array>

int main() {
  INIT_TEST("vccc::type_support")

  { // vtype_convert
    std::vector<float> v = {1.3, 2.6, 3.9};

    TEST_ENSURES(vccc::vtype_convert<int>(v)[0] == 1);
    
# if VCCC_USE_OPENCV_FEATURES
    TEST_ENSURES(vccc::vtype_convert<int>(v)[1] == 3);
    TEST_ENSURES(vccc::vtype_convert<int>(v)[2] == 4);
# else
    TEST_ENSURES(vccc::vtype_convert<int>(v)[1] == 2);
    TEST_ENSURES(vccc::vtype_convert<int>(v)[2] == 3);
# endif
  }

# if VCCC_USE_OPENCV_FEATURES
  { // convert_to (cv)
    
    cv::Point2f p(1.3,2.6);
    auto vec = vccc::convert_to<cv::Vec2i>(p);
    TEST_ENSURES(vccc::at<0>(vec) == 1);
    TEST_ENSURES(vccc::at<1>(vec) == 3);
    
    auto vec2 = vccc::convert_to<std::vector<int>>(p);
    TEST_ENSURES((vec2.size() == 2));
    TEST_ENSURES((vec2[0] == 1));
    TEST_ENSURES((vec2[1] == 3));
    
    auto vec3 = vccc::convert_to<cv::Vec3f>(p);
    TEST_ENSURES(std::get<0>(vec3) == std::get<0>(p));
    TEST_ENSURES(std::get<1>(vec3) == std::get<1>(p));
    TEST_ENSURES(std::get<2>(vec3) == 0);
  }
# endif

  { // convert_to (tuple_
    std::tuple<std::string> t = {"Hello, world!"};
    
    static_assert(vccc::detail::is_tuple_like<std::pair<std::string, std::string>>::value, " ");
    static_assert(vccc::detail::is_tuple_like<std::tuple<std::string>>::value, " ");
    
    
    auto t1 = vccc::convert_to<std::pair<std::string, std::string>>(t);
    
    TEST_ENSURES(t1.first == std::get<0>(t));
    TEST_ENSURES(t1.second.empty());
    
    auto t2 = vccc::convert_to<std::pair<std::string, std::string>>(std::move(t));
    
    TEST_ENSURES(std::get<0>(t).empty());
    TEST_ENSURES(t2.first == "Hello, world!");
    TEST_ENSURES(t2.second.empty());
    
  }

  { // reserved_vector
    auto v = vccc::reserved_vector<int>(123);
    TEST_ENSURES(v.capacity() == 123);
    TEST_ENSURES(v.empty());
  }

  static_assert(std::tuple_size<std::array<int, 3>>::value == 3, " " );

  return TEST_RETURN_RESULT;
}