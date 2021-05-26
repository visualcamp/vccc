//
// Created by YongGyu Lee on 2021/02/03.
//

#include <iostream>
#include <vccc/optional.hpp>
#include <vccc/test.hpp>

struct CopyConstructableTrivially {
  CopyConstructableTrivially() = default;
  CopyConstructableTrivially(CopyConstructableTrivially const&) = default;
  CopyConstructableTrivially(CopyConstructableTrivially &&) = default;
  CopyConstructableTrivially& operator=(CopyConstructableTrivially const&) = default;
  CopyConstructableTrivially& operator=(CopyConstructableTrivially &&) = default;
};
struct CopyConstructableButNotTrivial {
  CopyConstructableButNotTrivial() = default;
  CopyConstructableButNotTrivial(CopyConstructableButNotTrivial const&) {};
  CopyConstructableButNotTrivial(CopyConstructableButNotTrivial &&)  = default;
  CopyConstructableButNotTrivial& operator=(CopyConstructableButNotTrivial const&)  = default;
  CopyConstructableButNotTrivial& operator=(CopyConstructableButNotTrivial &&)  = default;
};
struct CopyConstructableForbidden {
  CopyConstructableForbidden() = default;
  CopyConstructableForbidden(CopyConstructableForbidden const&) = delete;
  CopyConstructableForbidden(CopyConstructableForbidden &&) = default;
  CopyConstructableForbidden& operator=(CopyConstructableForbidden const&) = default;
  CopyConstructableForbidden& operator=(CopyConstructableForbidden &&) = default;
};
struct MoveConstructableTrivially {
  MoveConstructableTrivially() = default;
  MoveConstructableTrivially(MoveConstructableTrivially const&) = default;
  MoveConstructableTrivially(MoveConstructableTrivially &&) = default;
  MoveConstructableTrivially& operator=(MoveConstructableTrivially const&) = default;
  MoveConstructableTrivially& operator=(MoveConstructableTrivially &&) = default;
};
struct MoveConstructableButNotTrivial {
  MoveConstructableButNotTrivial() = default;
  MoveConstructableButNotTrivial(MoveConstructableButNotTrivial const&) = default;
  MoveConstructableButNotTrivial(MoveConstructableButNotTrivial &&) {};
  MoveConstructableButNotTrivial& operator=(MoveConstructableButNotTrivial const&) = default;
  MoveConstructableButNotTrivial& operator=(MoveConstructableButNotTrivial &&) = default;
};
struct MoveConstructableForbidden {
  MoveConstructableForbidden() = default;
  MoveConstructableForbidden(MoveConstructableForbidden const&) {
    std::cout << "MCF COPY" << std::endl;
  }
  MoveConstructableForbidden(MoveConstructableForbidden &&) = delete;
  MoveConstructableForbidden& operator=(MoveConstructableForbidden const&) = default;
  MoveConstructableForbidden& operator=(MoveConstructableForbidden &&) = default;
};


template<typename T>
using optional = typename vccc::optional<T>;

int main() {
  INIT_TEST("vccc::optional")
  std::cout << std::boolalpha;

//  B b1, b2;
//  std::cout << std::is_trivially_constructible<B>{} << std::endl;

  static_assert(std::is_trivially_copy_constructible<CopyConstructableTrivially>::value, "");
  static_assert(std::is_copy_constructible<CopyConstructableButNotTrivial>::value &&
    !std::is_trivially_copy_constructible<CopyConstructableButNotTrivial>::value, "");
  static_assert(!std::is_copy_constructible<CopyConstructableForbidden>::value, "");

  static_assert(std::is_trivially_move_constructible<MoveConstructableTrivially>::value, "");
  static_assert(std::is_move_constructible<MoveConstructableButNotTrivial>::value &&
    !std::is_trivially_move_constructible<MoveConstructableButNotTrivial>::value, "");
  static_assert(!std::is_move_constructible<MoveConstructableForbidden>::value, "");

  // check
  static_assert(!std::is_copy_constructible<optional<CopyConstructableForbidden>>::value, "");
  static_assert(std::is_trivially_copy_constructible<optional<CopyConstructableTrivially>>::value, "");
  static_assert(std::is_constructible<optional<CopyConstructableTrivially>, const optional<CopyConstructableTrivially>&>::value, "");
  static_assert(std::is_constructible<optional<CopyConstructableButNotTrivial>, const optional<CopyConstructableButNotTrivial>&>::value, "");
  static_assert(!std::is_constructible<optional<CopyConstructableForbidden>, const optional<CopyConstructableForbidden>&>::value, "");
  static_assert(std::is_constructible<optional<MoveConstructableTrivially>, optional<MoveConstructableTrivially>&&>::value, "");
  static_assert(std::is_constructible<optional<MoveConstructableButNotTrivial>, optional<MoveConstructableButNotTrivial>&&>::value, "");
  static_assert(std::is_constructible<optional<MoveConstructableForbidden>, optional<MoveConstructableForbidden>&&>::value, "");
  static_assert(std::is_constructible<optional<int>, const optional<CopyConstructableForbidden>&>::value, "");

  static_assert(std::is_trivially_move_constructible<optional<MoveConstructableTrivially>>::value, "");
  MoveConstructableForbidden m;
  MoveConstructableForbidden m2(m);

  optional<CopyConstructableButNotTrivial> op;
  optional<int> op2;
  op2.has_value();

  MoveConstructableForbidden mcf;

  {
    vccc::optional<int> o1, // empty
    o2 = 1, // init from rvalue
    o3 = o2; // copy-constructor
    TEST_ENSURES(o1.has_value() == false);
    try {
      o1.value();
      TEST_ENSURES(false);
    } catch (const vccc::bad_optional_access& e) {
      TEST_ENSURES(true);
    }


    TEST_ENSURES(o2.has_value() == true);
    try {
      o2.value();
      TEST_ENSURES(true);
    } catch (const vccc::bad_optional_access& e) {
      TEST_ENSURES(false);
    }
    TEST_ENSURES(o2.value() == 1);

    // calls std::string( initializer_list<CharT> ) constructor
    vccc::optional<std::string> o4(vccc::in_place, {'a', 'b', 'c'});

    // calls std::string( size_type count, CharT ch ) constructor
    vccc::optional<std::string> o5(vccc::in_place, 3, 'A');

    // Move-constructed from std::string using deduction guide to pick the type

    vccc::optional<std::string> o6(std::string{"deduction"});

    std::cout << *o2 << ' ' << *o3 << ' ' << *o4 << ' ' << *o5  << ' ' << *o6 << '\n';
  }

  struct foo {};
  TEST_ENSURES(std::is_default_constructible<std::hash<vccc::optional<foo>>>::value == false);
  TEST_ENSURES(std::is_default_constructible<std::hash<vccc::optional<int>>>::value == true);

  return TEST_RETURN_RESULT;
}