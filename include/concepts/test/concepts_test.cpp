//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <array>

#include "vccc/concepts.hpp"

namespace vccc {
namespace {

int Test() {
  INIT_TEST("vccc::concepts")

  { // convertible_to
    struct From;
    struct To {
      explicit To(From&) = delete;
    };
    struct From {
      operator To();
    };

    static_assert(std::is_convertible<From&, To>::value, " ");
    static_assert(not concepts::convertible_to<From&, To>::value, " ");
  }



  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
