//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"
#include <cmath>
#include <vccc/math.hpp>
#include <array>

enum Event {
  UNINITIALIZED_CONSTRUCTED = 0,
  INITIALIZED_CONSTRUCTED,
  COPY_CONSTRUCTED,
  MOVE_CONSTRUCTED,
  COPY_ASSIGNED,
  MOVE_ASSIGNED,
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_MUL_SCALAR,

  EVENT_SIZE
};

template<typename T>
class OpChecker {
 public:
  static_assert(std::is_scalar<T>::value, "");

  T x;
  OpChecker() : x(0) { addEvent(UNINITIALIZED_CONSTRUCTED); }
  OpChecker(T x) : x(x) { addEvent(INITIALIZED_CONSTRUCTED); }

  OpChecker(const OpChecker& oc) : x(oc.x){ addEvent(COPY_CONSTRUCTED); }
  OpChecker(OpChecker&& oc) : x(oc.x){ addEvent(MOVE_CONSTRUCTED); }

  OpChecker& operator = (const OpChecker& oc) { x = oc.x; addEvent(COPY_ASSIGNED); return *this;}
  OpChecker& operator = (OpChecker&& oc)  noexcept { x = oc.x; addEvent(MOVE_ASSIGNED); return *this;}

  OpChecker& operator = (T&& val) {
    x = std::forward<T>(val);
    std::is_rvalue_reference<decltype(val)>::value ?
      addEvent(MOVE_ASSIGNED) :
      addEvent(COPY_ASSIGNED);
    return *this;
  }

  static std::vector<unsigned long> counter;

  static void addEvent(Event event) {
    assert(((void)"Not Event", 0 <= event && event < EVENT_SIZE));
    ++counter[event];
  }

  static auto getEvent(Event event) {
    assert(((void)"Not Event", 0 <= event && event < EVENT_SIZE));
    return counter[event];
  }

  static void reset_params() {
    if(counter.size() != EVENT_SIZE)
      counter.resize(EVENT_SIZE);
    for(auto& c : counter)
      c = 0;
  }

  static void print_status() {
    using std::cout;
    using std::endl;
    cout << "====================\n"
     << "Crt UnI: " << counter[UNINITIALIZED_CONSTRUCTED] << '\n'
     << "Crt Ini: " << counter[INITIALIZED_CONSTRUCTED] << '\n'
     << "Cp Ctor: " << counter[COPY_CONSTRUCTED] << '\n'
     << "Mv Ctor: " << counter[MOVE_CONSTRUCTED] << '\n'
     << "Cp Asgn: " << counter[COPY_ASSIGNED] << '\n'
     << "Mv Asgn: " << counter[MOVE_ASSIGNED] << '\n'
     << "Op Add : " << counter[OP_ADD] << '\n'
     << "Op Sub : " << counter[OP_SUB] << '\n'
     << "Op Mul : " << counter[OP_MUL] << '\n'
     << "Op MulS: " << counter[OP_MUL_SCALAR] << '\n'
     << "====================" << endl;
  }
};

template<typename T>
std::ostream& operator << (std::ostream& os, const OpChecker<T>& oc) {
  os << oc.x;
  return os;
}

template<typename T>
T operator + (const OpChecker<T>& x, const OpChecker<T>& y) { OpChecker<T>::addEvent(OP_ADD); return x.x + y.x; }
template<typename T>
OpChecker<T>& operator += (OpChecker<T>& x, const OpChecker<T>& y) { OpChecker<T>::addEvent(OP_ADD); x.x += y.x; return x; }
template<typename T>
OpChecker<T>& operator += (OpChecker<T>& x, const T& y) { OpChecker<T>::addEvent(OP_ADD); x.x += y; return x; }

template<typename T>
T operator - (const OpChecker<T>& x, const OpChecker<T>& y) { OpChecker<T>::addEvent(OP_SUB); return x.x - y.x; }
template<typename T>
T operator * (const OpChecker<T>& x, const OpChecker<T>& y) { OpChecker<T>::addEvent(OP_MUL); return x.x * y.x; }
template<typename T>
T operator / (const OpChecker<T>& x, const OpChecker<T>& y) { OpChecker<T>::addEvent(OP_MUL); return x.x / y.x; }
template<typename T>
T operator * (const OpChecker<T>& x, const T& y) { OpChecker<T>::addEvent(OP_MUL_SCALAR); return x.x * y; }
template<typename T>
T operator / (const OpChecker<T>& x, const T& y) { OpChecker<T>::addEvent(OP_MUL_SCALAR); return x.x / y; }

template<typename T>
std::vector<unsigned long> OpChecker<T>::counter(EVENT_SIZE, 0);

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

  constexpr inline bool operator != (Floating_ other) { return !(*this == other); }
  constexpr inline bool operator != (T other) { return !(*this == other); }

  T value;
};

template<typename T>
T Floating_<T>::eps = 1E-5;

template<typename T>
Floating_<T> Floating(T val) {
  return Floating_<T>(val);
}


struct foo {
  enum{ rows, cols };
};

struct bar {
  static foo rows, cols;
};

//template<typename E, int m, int n>
//volatile void ee(const vccc::MatExpression<E, m, n>& base) {
//
//}

#define STR_PRINT(x) std::cout << #x << " : " << (x) << std::endl

int main() {
  INIT_TEST("vccc::math")

  auto y = [](double x) { return x; };
  TEST_ENSURES((vccc::partialDiff<double, 0>(vccc::differential_symmetric_t{}, y, std::make_tuple(2.)) == Floating(1.)));

  vccc::Matrix<int, 3, 4> m;
  std::cout << m.rows << "x" << m.cols << std::endl;
  vccc::Matrix<int, 3, 3> m2(1,2,3,4,5,6,7,8,9);
  std::cout << m2 << std::endl;

  const auto m3 = m2 + m2 + m2 + m2 + m2 + m2;
  vccc::Matrix<int, 3, 3> m4 = m3;

  vccc::Matrix<int, 3, 3> m5 = -m4;

  std::cout << (m2 - m2) << std::endl;
  std::cout << (m2 * 10) << std::endl;
  std::cout << (m2 / 10.) << std::endl;
  vccc::Matrix<float, 3, 3> m6 = m2 / 10;
  std::cout << m6 << std::endl;

  auto M = vccc::Matrix<int, 3, 3>({1, 2, 3, 4, 5, 6, 7, 8, 9});
  constexpr vccc::Matrix<int, 3, 3> M2 = vccc::Matrix<int, 3, 3>::zeros();

  std::cout << M << std::endl;
  auto M3 = M;
  M3 *= 2;

  std::cout << "M3: " << M3 << std::endl;

  auto temp = M3 * 2;

  STR_PRINT(M==(M + decltype(M)::zeros()));
  STR_PRINT(M!=M2);
//  std::cout << (M != M2) << std::endl;

  std::cout << M << std::endl;
  std::cout << M*M << std::endl;
  M = M*M;
  std::cout << M << std::endl;

  vccc::Matrix<int, 2, 2> A;
//  M = A*A;

  namespace math = vccc::internal::math;

  auto U = M+(M*M) * 3;

  std::cout << math::is_alias_safe_v<decltype(M)> << std::endl;
  std::cout << math::is_alias_safe_v<decltype(M*M)> << std::endl;
  std::cout << math::is_alias_safe_v<decltype(U)> << std::endl;
  std::cout << math::is_alias_safe_v<decltype(M*3)> << std::endl;


//  TEST_ENSURES((vccc::is_matrix<vccc::MatExpression<int, 1, 2>>::value == true));
//
//  vccc::Matrix<int, 3, 3> m1({1,2,3,4,5,6,7,8,9});
//
//  TEST_ENSURES(m1.cols == 3 && m1.rows == 3);
//  TEST_ENSURES(vccc::is_matrix<decltype(m1)>::value == true);
//  TEST_ENSURES(vccc::is_matrix<decltype(m1 + m1)>::value == true);
//  TEST_ENSURES(vccc::is_matrix<decltype(m1 - m1)>::value == true);
//  TEST_ENSURES(vccc::is_matrix<decltype(m1 + m1)>::value == true);
//  TEST_ENSURES(vccc::is_matrix<int>::value == false);
//  TEST_ENSURES(vccc::is_matrix<foo>::value == false);
//  TEST_ENSURES(vccc::is_matrix<bar>::value == false);
//
//  TEST_ENSURES((m1 + m1 == vccc::Matrix<int, 3, 3>({2,4,6,8,10,12,14,16,18})));
//  TEST_ENSURES((m1 * 3 == vccc::Matrix<int, 3, 3>(3,6,9,12,15,18,21,24,27)));
//
//  // constexpr compilation check
//  constexpr vccc::Matrix<int, 3, 3> m2(1,2,3,4,5,6,7,8,9);
//  constexpr vccc::Matrix<int, 3, 3> m3({1,2,3,4,5,6,7,8,9});
//  constexpr vccc::Matrix<double, 8, 8> m4;
//  constexpr auto m5 = vccc::Matrix<int, 3, 3>::zeros();
//  constexpr auto m6 = vccc::Matrix<int, 3, 3>::ones();
//  constexpr auto m7 = vccc::Matrix<int, 3, 3>::eye();
//  constexpr auto m8 = vccc::Matrix<int, 3, 1>::all(99);
//  constexpr auto m9 = vccc::Matrix<int, 3, 3>::diag(m8);
//
//
//  vccc::Matrix<OpChecker<int>, 4, 4> M(1,0,0,0,
//                                  0,1,0,0,
//                                  0,0,1,0,
//                                  0,0,0,1);
//  TEST_ENSURES(OpChecker<int>::getEvent(INITIALIZED_CONSTRUCTED) == M.size);
//
//  volatile auto expr = M * 3;
//  TEST_ENSURES(OpChecker<int>::getEvent(INITIALIZED_CONSTRUCTED) == M.size &&
//               OpChecker<int>::getEvent(MOVE_ASSIGNED) == 0 &&
//               OpChecker<int>::getEvent(COPY_ASSIGNED) == 0 &&
//               OpChecker<int>::getEvent(COPY_CONSTRUCTED) == 0 &&
//               OpChecker<int>::getEvent(OP_MUL_SCALAR) == 0);
//
//  M = M * 3;
//  TEST_ENSURES(OpChecker<int>::getEvent(INITIALIZED_CONSTRUCTED) == M.size &&
//      OpChecker<int>::getEvent(MOVE_ASSIGNED) == 16 &&
//      OpChecker<int>::getEvent(COPY_ASSIGNED) == 0 &&
//      OpChecker<int>::getEvent(COPY_CONSTRUCTED) == 0 &&
//      OpChecker<int>::getEvent(OP_MUL_SCALAR) == 16);
//
//  vccc::Matrix<int, 3, 3> A = m1 * m1;
//  std::cout << A << std::endl;
//
//  vccc::Matrix<int, 2, 2> B, C;
//  B = vccc::Matrix<int, 2, 2>::ones();
//  C = vccc::Matrix<int, 2, 2>::ones();
//
//  OpChecker<int>::reset_params();
//  OpChecker<int>::print_status();
//  B = B * C;

//  cout << M << endl;
//  OpChecker<int>::print_status();
//
//  cout << M << endl;

//  m1 * m1;

  return TEST_RETURN_RESULT;
}