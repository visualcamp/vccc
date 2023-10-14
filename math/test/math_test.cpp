//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/math.hpp>
#include <array>

template<typename T>
struct Floating_ {
  static_assert(std::is_floating_point<T>::value, "T must be floating point");

  Floating_() : value() {}
  constexpr Floating_(T value) : value(value) {}
  constexpr operator T () { return value; }

  static constexpr inline void epsilon(T ep) { eps = ep;}
  static constexpr inline T epsilon() { return eps; }
  static constexpr T eps = static_cast<T>(1E-5);

  constexpr inline bool operator == (Floating_ other) {
    return std::abs(*this - other) * 2 <= epsilon();
  }
  constexpr inline bool operator == (T other) {
    return std::abs(*this - other) * 2 <= epsilon();
  }

  constexpr inline bool operator != (Floating_ other) { return !(*this == other); }
  constexpr inline bool operator != (T other) { return !(*this == other); }

  T value;
};

template<typename T>
constexpr Floating_<T> Floating(T val) {
  return Floating_<T>(val);
}

#define STR_PRINT(x) std::cout << #x << " : " << (x) << std::endl

int main() {
  INIT_TEST("vccc::math")

  auto y = [](double x) { return x; };
  TEST_ENSURES((vccc::partialDiff<double, 0>(vccc::differential_symmetric_t{}, y, std::make_tuple(2.)) == Floating(1.)));

  {
    constexpr vccc::Matrix33i m1;
    constexpr vccc::Matrix33i m2(1);
    constexpr vccc::Matrix33i m3(1,2,3);
    constexpr vccc::Matrix33i m4(1,2,3,4,5,6,7,8,9);
    constexpr vccc::Matrix33i m5 = m1;
    constexpr vccc::Matrix33i m6(m1);
    constexpr vccc::Matrix33i m7 = m1 + m2;
    vccc::Matrix33i m8({1,2,3});
    vccc::Matrix33i m9 = {1,2,3};
    vccc::Matrix33f m10 = m9.AsType<float>();
    vccc::Matrix<int, 1, 9> n1 = m9.Reshape<1>();
  }

  vccc::Matrix<int, 3, 4> m;
  TEST_ENSURES(m.rows == 3);
  TEST_ENSURES(m.cols == 4);

  vccc::Matrix<int, 3, 3> m2(1,2,3,4,5,6,7,8,9);
  const auto m3 = m2 + m2 + m2 + m2 + m2 + m2;
  TEST_ENSURES((m3 == vccc::Matrix<int, 3, 3>(6,12,18,24,30,36,42,48,54)));

  vccc::Matrix<int, 3, 3> m4 = m3;
  TEST_ENSURES(m4 == m3);

  TEST_ENSURES((m2 - m2 == vccc::Matrix<int, 3, 3>::zeros()));
  TEST_ENSURES((m2 * 10 == vccc::Matrix<int, 3, 3>(10,20,30,40,50,60,70,80,90)));
  TEST_ENSURES((m2 / 10 == vccc::Matrix<int, 3, 3>::zeros()));

  TEST_ENSURES((m2.AsType<float>() / 10 == vccc::Matrix<Floating_<float>, 3, 3>(0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f)));

  auto M = vccc::Matrix<int, 3, 3>({1, 2, 3, 4, 5, 6, 7, 8, 9});
  TEST_ENSURES((M == vccc::Matrix<int, 3, 3>(1,2,3,4,5,6,7,8,9)));

  TEST_ENSURES((vccc::Matrix<int, 3, 3>::zeros() == vccc::Matrix<int, 3, 3>(0,0,0, 0,0,0, 0,0,0)));
  TEST_ENSURES((vccc::Matrix<int, 3, 3>::ones() == vccc::Matrix<int, 3, 3>(1,1,1, 1,1,1, 1,1,1)));
  TEST_ENSURES((vccc::Matrix<int, 3, 3>::all(3) == vccc::Matrix<int, 3, 3>(3,3,3, 3,3,3, 3,3,3)));
  TEST_ENSURES((vccc::Matrix<int, 3, 3>::diag({1,2,3}) == vccc::Matrix<int, 3, 3>(1,0,0, 0,2,0, 0,0,3)));
  TEST_ENSURES((vccc::Matrix<int, 3, 4>::diag({1,2,3}) == vccc::Matrix<int, 3, 4>(1,0,0,0, 0,2,0,0, 0,0,3,0)));
  TEST_ENSURES((vccc::Matrix<int, 4, 3>::diag({1,2,3}) == vccc::Matrix<int, 4, 3>(1,0,0, 0,2,0, 0,0,3, 0,0,0)));

  auto M3 = M;
  M3 *= 2;
  TEST_ENSURES((M3 == M * 2));
  TEST_ENSURES((M == M + decltype(M)::zeros()));

  TEST_ENSURES(M * decltype(M)::zeros() == decltype(M)::zeros());
  vccc::Matrix33i::eye();
//  TEST_ENSURES(M * decltype(M)::eye() == M);

  using Matrix33i = vccc::Matrix<int, 3, 3>;
  TEST_ENSURES(Matrix33i(7,49,73, 58,30,72, 44,78,23) * Matrix33i(9,40,65, 92,42,87, 3,27,29) ==
      Matrix33i(4790,4309,6835, 3498,5524,8468, 7641,5657,10313));

  return TEST_RETURN_RESULT;
}
