//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <array>
#include <vccc/log.hpp>

#define ee 192829336

#if ee
#define val 1
#else
#define val 0
#endif

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



  return TEST_RETURN_RESULT;
}