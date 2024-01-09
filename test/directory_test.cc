//
// Created by YongGyu Lee on 2021/06/02.
//

#include <iostream>
#include <filesystem>

#include "vccc/directory.hpp"
#include "test_core.hpp"

namespace fs = std::filesystem;

int main() {
  INIT_TEST("vccc::directory")

  {
    vccc::directory dir;

    fs::recursive_directory_iterator rdi;
    fs::directory_iterator di;

    // compile test
    dir = rdi;
    TEST_ENSURES(dir.is_recursive() == true);

    dir = di;
    TEST_ENSURES(dir.is_recursive() == false);

    // compile test
//    dir.for_each([](const fs::directory_entry &entry) { ;
//    });
  }

  {
    vccc::directory dir(fs::current_path());

    int sum = 0;
    dir.for_each([&](const auto& entry){
      std::cout << entry.path() << '\n';
      ++sum;
    });
    TEST_ENSURES(sum > 0);
  }


  return TEST_RETURN_RESULT;
}
