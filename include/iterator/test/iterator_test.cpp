//
// Created by YongGyu Lee on 2021/02/03.
//

#include "test_core.hpp"

#include <cmath>
#include <forward_list>
#include <limits>
#include <list>
#include <string>
#include <map>
#include <vector>

#include "vccc/iterator.hpp"

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
    vccc::random_access_iterator<void> a;
    static_assert(vccc::random_access_iterator<int*>::value, "");
    static_assert(vccc::contiguous_iterator<int*>::value, "");
    static_assert(vccc::random_access_iterator<int**>::value, "");
    static_assert(vccc::random_access_iterator<void*>::value == false, "");

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

  return TEST_RETURN_RESULT;
}
