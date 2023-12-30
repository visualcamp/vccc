//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"

#include <algorithm>
#include <cmath>
#include <forward_list>
#include <limits>
#include <list>
#include <string>
#include <map>
#include <vector>

#include "vccc/iterator.hpp"

template<class T>
struct Holder {
  T t;
};

struct Incomplete;

using P = Holder<Incomplete>*;

int main() {
  INIT_TEST("vccc::iterator")

  {
    struct foo {};
    static_assert(std::numeric_limits<foo>::is_specialized == false, "");
  }

  {
    int* p;
    static_assert(vccc::weakly_incrementable<int*>::value, "");

    // operator==(vccc::unreachable_sentinel, p);
    (void)(vccc::unreachable_sentinel == p);
  }

  {
    std::vector<std::string> p = {"one", "two", "three"}, q;
    q.emplace_back(iter_move(std::make_move_iterator(p.rbegin())));

    TEST_ENSURES(p[2].empty());
    TEST_ENSURES(q[0] == "three");
  }

  {
    static_assert(vccc::indirectly_readable<void>::value == false, "");
    static_assert(vccc::indirectly_readable<int>::value == false, "");
    static_assert(vccc::indirectly_readable<int*>::value == true, "");
    static_assert(vccc::indirectly_readable<void*>::value == false, "");
    static_assert(vccc::indirectly_readable<std::vector<std::vector<int>>::iterator>::value == true, "");
    static_assert(vccc::indirectly_readable<std::vector<std::vector<int>>::const_iterator>::value == true, "");
    static_assert(vccc::indirectly_readable<std::vector<std::vector<int>>::reverse_iterator>::value == true, "");
    static_assert(vccc::indirectly_readable<std::move_iterator<std::vector<int>::iterator>>::value == true, "");
  }

  {
    static_assert(vccc::random_access_iterator<int*>::value, "");
    static_assert(vccc::contiguous_iterator<int*>::value, "");
    static_assert(vccc::random_access_iterator<int**>::value, "");
    static_assert(vccc::random_access_iterator<void*>::value == false, "");
    static_assert(vccc::random_access_iterator<void>::value == false, "");

    static_assert(vccc::input_or_output_iterator<std::vector<int>::iterator>::value, "");
    static_assert(vccc::input_iterator<std::vector<int>::iterator>::value, "");
    static_assert(vccc::bidirectional_iterator<std::vector<int>::iterator>::value, "");
    static_assert(vccc::random_access_iterator<std::vector<int>::iterator>::value, "");
    static_assert(vccc::contiguous_iterator<std::vector<int>::iterator>::value, "");
    static_assert(vccc::contiguous_iterator<std::vector<bool>::iterator>::value == false, "");


    static_assert(vccc::input_or_output_iterator<std::list<int>::iterator>::value, "");
    static_assert(vccc::input_iterator<std::list<int>::iterator>::value, "");
    static_assert(vccc::bidirectional_iterator<std::list<int>::iterator>::value, "");
    static_assert(vccc::random_access_iterator<std::list<int>::iterator>::value == false, "");
    static_assert(vccc::contiguous_iterator<std::list<int>::iterator>::value == false, "");

    static_assert(vccc::bidirectional_iterator<std::forward_list<int>::iterator>::value == false, "");
    static_assert(vccc::random_access_iterator<std::forward_list<int>::iterator>::value == false, "");
    static_assert(vccc::contiguous_iterator<std::forward_list<int>::iterator>::value == false, "");
  }

  {
    int arr[] = {1, 2, 3};
    std::vector<int> v = {4, 5, 6};
    std::list<int> l = {7, 8, 9};

    auto it1 = vccc::ranges::begin(arr);
    vccc::ranges::advance(it1, 1);
    TEST_ENSURES(*it1 == 2);
    vccc::ranges::advance(it1, (std::ptrdiff_t)1);


    auto it2 = v.rbegin();
    vccc::ranges::advance(it2, 2);
    TEST_ENSURES(*it2 == 4);

    auto it3 = l.begin();
    vccc::ranges::advance(it3, 1);
    TEST_ENSURES(*it3 == 8);

    vccc::ranges::advance(it3, 2);
    TEST_ENSURES(it3 == vccc::ranges::end(l));
  }

  {
    static_assert(vccc::indirectly_comparable<P*, P*, std::equal_to<>>::value, "");

    P a[10] = {}; // ten null pointers
    TEST_ENSURES(std::count(a, a + 10, nullptr) == 10); // OK
    // TODO: Implement ranges::count
    // TEST_ENSURES(std::ranges::count(a, a + 10, nullptr) == 10); // Error before C++26
  }

  return TEST_RETURN_RESULT;
}
