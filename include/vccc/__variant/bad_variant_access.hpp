//
// Created by YongGyu Lee on 11/3/23.
//

#ifndef VCCC_VARIANT_BAD_VARIANT_ACCESS_HPP
#define VCCC_VARIANT_BAD_VARIANT_ACCESS_HPP

#include <exception>

namespace vccc {

/// @addtogroup variant
/// @{

class bad_variant_access : public std::exception {
 public:
  bad_variant_access() noexcept = default;
  bad_variant_access(const bad_variant_access&) noexcept = default;

  const char* what() const noexcept override {
    return "vccc::variant: Bad variant access";
  }
};

/// @}

} // namespace vccc

#endif // VCCC_VARIANT_BAD_VARIANT_ACCESS_HPP
