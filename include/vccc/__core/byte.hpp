//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_CORE_BYTE_HPP
#define VCCC_CORE_BYTE_HPP

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

// #include "vccc/__type_traits/"

namespace vccc {

/// @addtogroup core
/// @{

// TODO: Use CheckCXXTypeExists
#if __cplusplus < 201703L

namespace detail {
struct byte_ctor_tag {};
}

class byte {
 public:
  byte() noexcept = default;

  constexpr explicit byte(std::initializer_list<unsigned char> il)
      : value_(*il.begin()) {
    assert(((void)"Size must be one", il.size() == 1));
  }

  // Internal use only
  constexpr explicit byte(unsigned char c, detail::byte_ctor_tag) noexcept : value_(c) {}

  constexpr explicit operator unsigned char() const noexcept {
    return value_;
  }

 private:
  unsigned char value_;
};

template<typename IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, IntegerType>
to_integer(byte b) noexcept {
  return IntegerType(b.operator unsigned char());
}

template <class IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, byte>
operator<<(byte b, IntegerType shift) noexcept {
  return byte(
      static_cast<unsigned char>(
        static_cast<unsigned int>(b.operator unsigned char()) << shift
      ),
      detail::byte_ctor_tag{}
  );
}

template <class IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, byte>
operator>>(byte b, IntegerType shift) noexcept {
  return byte(
      static_cast<unsigned char>(
        static_cast<unsigned int>(b.operator unsigned char()) >> shift
      ),
      detail::byte_ctor_tag{}
  );
}

template <class IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, byte&>
operator<<=(byte& b, IntegerType shift) noexcept {
  return b = b << shift;
}

template <class IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, byte&>
operator>>=(byte& b, IntegerType shift) noexcept {
  return b = b >> shift;
}

constexpr byte operator|(byte l, byte r) noexcept {
  return byte(
    static_cast<unsigned char>(
      static_cast<unsigned int>(l.operator unsigned char()) | static_cast<unsigned int>(r.operator unsigned char())
    ),
    detail::byte_ctor_tag{}
  );
}

constexpr byte operator&(byte l, byte r) noexcept {
  return byte(
    static_cast<unsigned char>(
      static_cast<unsigned int>(l.operator unsigned char()) & static_cast<unsigned int>(r.operator unsigned char())
    ),
    detail::byte_ctor_tag{}
  );
}

constexpr byte operator^(byte l, byte r) noexcept {
  return byte(
    static_cast<unsigned char>(
      static_cast<unsigned int>(l.operator unsigned char()) ^ static_cast<unsigned int>(r.operator unsigned char())
    ),
    detail::byte_ctor_tag{}
  );
}
constexpr byte operator~(byte b) noexcept {
  return byte(
      static_cast<unsigned char>(
        ~static_cast<unsigned int>(b.operator unsigned char())
      ),
      detail::byte_ctor_tag{}
  );
}

constexpr byte& operator|=(byte& l, byte r) noexcept {
  return l = l | r;
}

constexpr byte& operator&=(byte& l, byte r) noexcept {
  return l = l & r;
}

constexpr byte& operator^=(byte& l, byte r) noexcept {
  return l = l ^ r;
}
#else
using byte = std::byte;

template<typename IntegerType>
constexpr std::enable_if_t<std::is_integral<IntegerType>::value, IntegerType>
to_integer(byte b) noexcept {
  return IntegerType(b);
}

#endif

/// @}

} // namespace vccc

#endif // VCCC_CORE_BYTE_HPP
