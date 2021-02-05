//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/math.hpp>

template<typename T>
struct Floating_ {
  constexpr explicit Floating_(T value) : value(value) {}
  constexpr operator T () { return value; }

  static constexpr inline void epsilon(T ep) { eps = ep;}
  static constexpr inline T epsilon() { return eps; }
  static T eps;

  constexpr inline bool operator == (Floating_ other) {
    return std::abs(*this - other) <= epsilon() / 2.;
  }
  constexpr inline bool operator == (T other) {
    return std::abs(*this - other) <= epsilon() / 2.;
  }

  T value;
};

template<typename T>
T Floating_<T>::eps = 1E-5;

template<typename T>
Floating_<T> Floating(T val) {
  return Floating_<T>(val);
}

int main() {
  INIT_TEST("vccc::math")

  auto y = [](double x) { return x; };

  TEST_ENSURES((vccc::partialDiff<double, 0>(vccc::differential_symmetric_t{}, y, std::make_tuple(2.)) == Floating(1.)));

  vccc::Matrix<int, 3, 3> m1;
  vccc::Matrix<int, 3, 3> m2;

  std::cout << m1.cols << " x " << m1.rows << std::endl;
  std::cout << m2.cols << " x " << m2.rows << std::endl;
  static_assert(m1.cols == m2.cols, "");
  int x = 3;

  m1 + m2;

  return TEST_RETURN_RESULT;
}