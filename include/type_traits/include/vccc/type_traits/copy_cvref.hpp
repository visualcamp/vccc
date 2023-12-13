//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_TYPE_TRAITS_COPY_CVREF_HPP_
#define VCCC_TYPE_TRAITS_COPY_CVREF_HPP_

#include <type_traits>

namespace vccc {

/// @addtogroup type_traits
/// @{


/**
 * copy_const
 * @tparam From
 * @tparam To
 */
template<typename From, typename To>
struct copy_const {
  using type = To;
};

template<typename From, typename To>
struct copy_const<const From, To> {
  using type = std::add_const_t<To>;
};

template<typename From, typename To>
using copy_const_t = typename copy_const<From, To>::type;

/**
 * copy_volatile
 * @tparam From
 * @tparam To
 */
template<typename From, typename To>
struct copy_volatile {
  using type = To;
};

template<typename From, typename To>
struct copy_volatile<volatile From, To> {
  using type = std::add_volatile_t<To>;
};

template<typename From, typename To>
using copy_volatile_t = typename copy_volatile<From, To>::type;

/**
 * copy_cv
 * @tparam From
 * @tparam To
 */
template<typename From, typename To>
struct copy_cv {
  using type = copy_volatile_t<From, copy_const_t<From, To>>;
};

template<typename From, typename To>
using copy_cv_t = typename copy_cv<From, To>::type;

/**
 * copy_reference
 * @tparam From
 * @tparam To
 */
template<typename From, typename To>
struct copy_reference {
  using type = To;
};

template<typename From, typename To>
struct copy_reference<From&, To> {
  using type = std::add_lvalue_reference_t<To>;
};

template<typename From, typename To>
struct copy_reference<From&&, To> {
  using type = std::add_rvalue_reference_t<To>;
};

template<typename From, typename To>
using copy_reference_t = typename copy_reference<From, To>::type;

/**
 * copy_cvref
 * @tparam From
 * @tparam To
 */
template<typename From, typename To>
struct copy_cvref {
  using type = copy_reference_t<From, copy_cv_t<From, To>>;
};

template<typename From, typename To>
using copy_cvref_t = typename copy_cvref<From, To>::type;

/// @} type_traits

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_COPY_CVREF_HPP_
