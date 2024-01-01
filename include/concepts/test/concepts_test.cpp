//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <array>

#include "vccc/concepts.hpp"

#include <vccc/type_traits/has_typename_value_type.hpp>

namespace {

template<typename D>
struct crtp_base {
  // _$vccc_derived is defined in view_interface, etc
  using _$vccc_derived = D;
};

int Test() {
  INIT_TEST("vccc::concepts")

  {
    struct A {
      explicit A(int) {}
      A& operator=(int&) { return *this; }
      int data;
    };
    struct B {
      B(int) {}
      B& operator=(int&) { return *this; }
      int data;
    };

    static_assert(vccc::assignable_from<A&, int>::value == false, "");
    static_assert(vccc::assignable_from<B&, int>::value == true, "");

    static_assert(vccc::assignable_from<int&, char>::value, "");
    static_assert(vccc::assignable_from<int&, long>::value, "");
    static_assert(vccc::assignable_from<int&, float>::value, "");
    static_assert(vccc::assignable_from<int&, double>::value, "");
    static_assert(vccc::assignable_from<int&, int&&>::value, "");
    static_assert(vccc::assignable_from<int&, int*>::value == false, "");
    static_assert(vccc::assignable_from<int&, void>::value == false, "");
    static_assert(vccc::assignable_from<int&, void*>::value == false, "");
  }

  { // convertible_to
    struct From;
    struct To {
      explicit To(From&) = delete;
    };
    struct From {
      operator To();
    };

    static_assert(std::is_convertible<From&, To>::value, " ");
    static_assert(not vccc::convertible_to<From&, To>::value, " ");
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

  {
    struct A : crtp_base<int> {};
    struct B : crtp_base<int>, crtp_base<double> {};

    static_assert(vccc::derived_from_single_crtp<void, crtp_base>::value == false, "");
    static_assert(vccc::derived_from_single_crtp<int, crtp_base>::value == false, "");
    static_assert(vccc::derived_from_single_crtp<A, crtp_base>::value, "");
    static_assert(vccc::derived_from_single_crtp<B, crtp_base>::value == false, "");
  }

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
