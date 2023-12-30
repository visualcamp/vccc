//
// Created by YongGyu Lee on 2021/02/03.
//

#include <array>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "vccc/span.hpp"
#include "vccc/test.hpp"

namespace {

int Test() {
  INIT_TEST("vccc::span")

  {
    int array[] = {1, 2, 3};
    vccc::span<int> s(array);

    TEST_ENSURES(s.size() == 3);
    TEST_ENSURES(s.size_bytes() == 3 * sizeof(int));
    TEST_ENSURES(s.data() == array);
    TEST_ENSURES(s[0] == array[0]);
    TEST_ENSURES(s[1] == array[1]);
    TEST_ENSURES(s[2] == array[2]);
    TEST_ENSURES(s.begin() == std::begin(array));
    TEST_ENSURES(s.end() == std::end(array));
    TEST_ENSURES(*std::rbegin(array) == 3);
    TEST_ENSURES(*s.rbegin() == 3);
    TEST_ENSURES(s.rbegin() == std::rbegin(array));
    TEST_ENSURES(s.rend() == std::rend(array));
    TEST_ENSURES(s.front() == 1);
    TEST_ENSURES(s.back() == 3);
    TEST_ENSURES(s.first<1>().size() == 1);
    TEST_ENSURES(s.first<1>()[0] == 1);
    TEST_ENSURES(s.first<2>().size() == 2);
    TEST_ENSURES(s.first<2>().back() == 2);
    TEST_ENSURES(s.last<2>().front() == 2);
  }

  {
    std::vector<int> v = {1, 2, 3};
    vccc::span<int> s(v);

#if __cplusplus >= 201703L
    vccc::span s2(v);
    vccc::span s3(v.begin(), v.end());

    int array[] = {1, 2, 3};
    vccc::span s4(array);

    int* p1 = array;
    int* p2 = array + 3;
    vccc::span s5(p1, p2);
#endif

    TEST_ENSURES(s.size() == 3);
    TEST_ENSURES(s.size_bytes() == 3 * sizeof(int));
    TEST_ENSURES(s.data() == v.data());
    TEST_ENSURES(s[0] == v[0]);
    TEST_ENSURES(s[1] == v[1]);
    TEST_ENSURES(s[2] == v[2]);
    TEST_ENSURES(*std::rbegin(v) == 3);
    TEST_ENSURES(*s.rbegin() == 3);
    TEST_ENSURES(s.front() == 1);
    TEST_ENSURES(s.back() == 3);
    TEST_ENSURES(s.first<1>().size() == 1);
    TEST_ENSURES(s.first<1>()[0] == 1);
    TEST_ENSURES(s.first<2>().size() == 2);
    TEST_ENSURES(s.first<2>().back() == 2);
    TEST_ENSURES(s.last<2>().front() == 2);
  }

  {
    TEST_ENSURES((std::is_constructible<vccc::span<int>, std::vector<int>&>::value));
    TEST_ENSURES((std::is_constructible<vccc::span<int>, int (&)[3]>::value));
    TEST_ENSURES((std::is_constructible<vccc::span<int, 3>, int (&)[3]>::value));
    TEST_ENSURES((std::is_constructible<vccc::span<int, 4>, int (&)[3]>::value == false));
    TEST_ENSURES((std::is_constructible<vccc::span<int>, std::map<int, int>>::value == false));
    TEST_ENSURES((std::is_constructible<vccc::span<int>, std::list<int>>::value == false));
  }

  {
    auto print = [](float const x, vccc::span<const vccc::byte> const bytes) {
      std::cout << std::setprecision(6) << std::setw(8) << x << " = { "
                << std::hex << std::uppercase << std::setfill('0');
      for (auto const b : bytes)
        std::cout << std::setw(2) << vccc::to_integer<int>(b) << ' ';
      std::cout << std::dec << "}\n";
    };
    /* mutable */ float data[1]{3.141592f};

    auto const const_bytes = vccc::as_bytes(vccc::span<float, 1>{data});
    print(data[0], const_bytes);

    auto const writable_bytes = vccc::as_writable_bytes(vccc::span<float, 1>{data});

    // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
    writable_bytes[3] |= vccc::byte{0B1000'0000};
    print(data[0], const_bytes);

  }

  return TEST_RETURN_RESULT;
}

} // namespace

int main() {
  return ::Test();
}
