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

  LOGD(1);

  std::cout << std::boolalpha;


  char str[10] = {'h', 'e', 'l', 'l', 'o'};

  LOGD("%s", str);
  LOGD("%p", str);

  std::cout << vccc::detail::are_types_c_printable<const char*, char[]>() << std::endl;
  std::cout << vccc::detail::are_types_c_printable<const char*, char*>() << std::endl;
  std::cout << vccc::detail::are_types_c_printable<const char*, class t>() << std::endl;


  LOGD("pwd: ");
  LOGD("pwd: ", PWD);
  LOGD(VCCC_FILE_SEPARATOR);
  LOGD(BOOST_COMP_MSVC);
  LOGD(BOOST_VERSION_NUMBER_NOT_AVAILABLE);
  LOGD(BOOST_COMP_CLANG);

  std::ratio<3> r1, r2;

  LOGD(std::ratio_equal<decltype(r1), decltype(r2)>::value);

  auto h = std::chrono::hours(10000000);
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(h).count() << std::endl;

  auto d = std::chrono::nanoseconds(1);
  for(int i=0; i<50; ++i) {
    LOGE(d.count(), ":", vccc::stringfy(d));
    d *= 2;
  }


  std::cout << PWD << std::endl;

  return TEST_RETURN_RESULT;
}