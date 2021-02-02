//
// Created by cosge on 2021-02-02.
//

#include "test_core.hpp"
#include <tuple>
#include <vccc/high.hpp>


int main(){
  INIT_TEST("vccc::high")


  TEST_ENSURES(1 == 1);
//  TEST_ENSURES(1 == 2);
  TEST_ENSURES(2 == 2);

  return TEST_RETURN_RESULT;
}