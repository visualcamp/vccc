//
// Created by YongGyu Lee on 02/01/24.
//

#ifndef VCCC_VARIANT_HASH_HPP
#define VCCC_VARIANT_HASH_HPP

#include <functional>

#include "vccc/__functional/hash_array.hpp"
#include "vccc/__variant/variant.hpp"
#include "vccc/__variant/variant_npos.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

/// @addtogroup variant
/// @{

namespace vccc {
namespace detail {

struct variant_hash_visitor {
  template<typename T, std::size_t I>
  constexpr std::size_t operator()(const T& x, in_place_index_t<I>) const {
    return vccc::FNV_1a(std::hash<T>{}(x), I);
  }

  template<typename T>
  constexpr std::size_t operator()(const T& x, in_place_index_t<variant_npos>) const {
    return 0;
  }
};

template<typename Variant, bool Hashable /* false */>
struct variant_hash {};

template<typename Variant>
struct variant_hash<Variant, true> {
  std::size_t operator()(const Variant& var) const {
    return variant_raw_visit(var.index(), var._base().union_, variant_hash_visitor{});
  }
};

} // namespace detail
} // namespace vccc

template<typename... Ts>
struct std::hash<vccc::variant<Ts...>>
    : vccc::detail::variant_hash<
        vccc::variant<Ts...>,
        vccc::conjunction<vccc::is_invocable<std::hash<Ts>, const vccc::remove_cvref_t<Ts>&>...>::value
      > {};

/// @}

#endif // VCCC_VARIANT_HASH_HPP
