//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/log.hpp>
#include <array>


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





  return TEST_RETURN_RESULT;
}