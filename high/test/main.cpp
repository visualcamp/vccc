//
// Created by YongGyu Lee on 2020/12/29.
//

#include <iostream>
#include "vccc/high.hpp"

int main(){
  vccc::invoke([](int a){}, 1);

  std::cout << "Hello, world!" << std::endl;
}