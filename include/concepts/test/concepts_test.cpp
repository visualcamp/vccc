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
    static_assert(not convertible_to<From&, To>::value, " ");
  }

  {
    static_assert(vccc::boolean_testable<bool>::value, "");
    static_assert(vccc::boolean_testable<std::true_type>::value, "");
    static_assert(vccc::boolean_testable<std::false_type>::value, "");
    static_assert(vccc::boolean_testable<int*>::value, "");
    static_assert(vccc::boolean_testable<void>::value == false, "");
  }

  {
    struct F {
      void operator()(int&) const {}
    };

    static_assert(vccc::invocable<F, void>::value == false, "");
    static_assert(vccc::invocable<F, int>::value == false, "");
    static_assert(vccc::invocable<F, int&>::value, "");
    static_assert(vccc::invocable<F, int&&>::value == false, "");
  }

  return TEST_RETURN_RESULT;
}

} // namespace
} // namespace vccc

int main() {
  return ::vccc::Test();
}
