//
// Created by YongGyu Lee on 11/27/23.
//

#ifndef VCCC_STRING_VIEW_HPP_
#define VCCC_STRING_VIEW_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <ostream>

#if __cplusplus >= 201703L
#include <string_view>
#endif

#include "vccc/__config.h"
#include "vccc/__functional/hash_array.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__memory/to_address.hpp"
#include "vccc/__ranges/borrowed_range.hpp"
#include "vccc/__ranges/contiguous_range.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/enable_view.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/type_identity.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

/**
@defgroup string_view string_view
@brief A lightweight object that offers read-only access to a string or a part of a string
*/

namespace vccc {

namespace detail {

template<
    typename It,
    typename End,
    typename CharT,
    typename SizeType,
    bool = conjunction<
               contiguous_iterator<It>,
               sized_sentinel_for<End, It>,
               has_typename_type<iter_value<It>>,
               negation<std::is_convertible<It, SizeType>>
           >::value /* true */
>
struct string_view_iter_ctor : std::is_same<iter_value_t<It>, CharT> {};

template<typename It, typename End, typename CharT, typename SizeType>
struct string_view_iter_ctor<It, End, CharT, SizeType, false> : std::false_type {};

template<typename D, typename SV, typename = void>
struct has_operator_string_view
#if __cplusplus < 202002L
  : std::is_same<D, std::string> {};
#else
  : std::false_type {};
#endif
template<typename D, typename SV>
struct has_operator_string_view<D, SV, void_t<decltype( std::declval<D&>().operator SV() )>> : std::true_type {};

template<
    typename R,
    typename SV,
    bool = conjunction<
               negation< std::is_same<remove_cvref_t<R>, SV> >,
               ranges::contiguous_range<R>,
               ranges::sized_range<R>,
               has_typename_type<ranges::range_value<R>>
           >::value /* true */
>
struct string_view_range_ctor
    : conjunction<
          std::is_same<ranges::range_value_t<R>, typename SV::value_type>,
          negation< std::is_convertible<R, typename SV::const_pointer> >,
          negation< has_operator_string_view<remove_cvref_t<R>, SV> >
      >{};
template<typename R, typename SV>
struct string_view_range_ctor<R, SV, false> : std::false_type {};

} // namespace detail

//! @addtogroup string_view
//! @{

/**
 * \brief The templated class `vccc::basic_string_view` provides a lightweight object
 * that offers read-only access to a string or a part of a string using an interface
 * similar to the interface of `std::basic_string`.
 *
 * The class template basic_string_view describes an object that can refer to a constant
 * contiguous sequence of `CharT` with the first element of the sequence at position zero.
 * Every specialization of `vccc::basic_string_view` is a TriviallyCopyable type.
 *
 * \tparam CharT character type
 * \tparam Traits CharTraits class specifying the operations on the character type.
 * Like for `std::basic_string`, `Traits::char_type` must name the same type as `CharT`
 * or the program is ill-formed.
 */
template<
    typename CharT,
    typename Traits = std::char_traits<CharT>>
class basic_string_view {
 public:
  using traits_type = Traits;
  using value_type = CharT;
  using pointer = CharT*;
  using const_pointer = const CharT*;
  using reference = CharT&;
  using const_reference = const CharT&;
  using const_iterator = const_pointer;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  static constexpr size_type npos = size_type(-1);

  /**
   * \name (constructor)
   * \brief constructs a `basic_string_view`
   * @{
   */
  constexpr basic_string_view() noexcept
      : data_(nullptr), size_(0) {}

  constexpr basic_string_view( const basic_string_view& other ) noexcept = default;

  constexpr basic_string_view( const CharT* s, size_type count )
      : data_(s), size_(count) {}

  constexpr basic_string_view( const CharT* s )
      : data_(s), size_(traits_type::length(s)) {}

  template<typename It, typename End, std::enable_if_t<
      detail::string_view_iter_ctor<It, End, CharT, size_type>::value, int> =0>
  constexpr basic_string_view(It first, End last)
      : data_(vccc::to_address(first)), size_(last - first) {}

  template<typename  R, std::enable_if_t<detail::string_view_range_ctor<R, basic_string_view>::value, int> = 0>
  constexpr explicit basic_string_view(R&& r)
      : data_(ranges::data(r)), size_(ranges::size(r)) {}


  // basic_string_view does not have a constructor that accepts std::basic_string.
  // Rather, std::basic_string defines a operator string_view.
  // Add two custom constructors since std::basic_string cannot be modified
  // It is the programmer's responsibility to ensure that the resulting string view does not outlive the string.
  constexpr basic_string_view(const std::basic_string<CharT, Traits>& s)
  : data_(s.data()), size_(s.size()) {}

  constexpr basic_string_view(std::basic_string<CharT, Traits>&& s)
      : data_(s.data()), size_(s.size()) {}

#if __cplusplus < 201703L
  // Substitutaion for std::basic_string::basic_string(StringViewLike, ...)
  explicit operator std::basic_string<CharT, Traits>() const {
    return std::basic_string<CharT, Traits>(data(), size());
  }
#else
  operator std::basic_string_view<CharT, Traits>() const {
    return std::basic_string_view<CharT, Traits>(data(), size());
  }
#endif

  constexpr basic_string_view(std::nullptr_t) = delete;
  /// @}

  /**
   * \name (operator=)
   * \brief assigns a view
   * @{
   */
  constexpr basic_string_view& operator=(const basic_string_view& other) noexcept = default;
  /// @}

  /**
   * \name begin/cbegin
   * \brief returns an iterator to the beginning
   * @{
   */
  constexpr const_iterator begin() const noexcept { return data_; }

  constexpr const_iterator cbegin() const noexcept { return data_; }
  /// @}

  /**
   * \name end/cend
   * \brief returns an iterator to the end
   * @{
   */
  constexpr const_iterator end() const noexcept { return data_ + size_; }

  constexpr const_iterator cend() const noexcept { return data_ + size_; }
  /// @}

  /**
   * \name rbegin/crbegin
   * \brief returns a reverse iterator to the beginning
   * @{
   */
  constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }

  constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
  /// @}

  /**
   * \name rend
   * \brief returns a reverse iterator to the end
   * @{
   */
  constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }

  constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
  /// @}

  /**
   * \name operator[]
   * \brief accesses the specified character
   */

  /**
   * Returns a const reference to the character at specified location pos.
   * No bounds checking is performed: the behavior is undefined if `pos >= size()`.
   *
   * \param pos position of the character to return
   * \return Const reference to the requested character.
   */
  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  /**
   * \name at
   * \brief accesses the specified character with bounds checking
   */
  constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("vccc::string_view::at : out of range");
    }
    return (*this)[pos];
  }

  /**
   * \name front
   * \brief accesses the first character
   */
  constexpr const_reference front() const { return (*this)[0]; }

  /**
   * \name back
   * \brief accesses the last character
   */
  constexpr const_reference back() const { return (*this)[size() - 1]; }

  /**
   * \name data
   * \brief returns a pointer to the first character of a view
   */
  constexpr const_pointer data() const noexcept { return data_; }

  /**
   * \name size/length
   * \brief returns the number of characters
   * @{
   */
  constexpr size_type size() const noexcept { return size_; }

  constexpr size_type length() const noexcept { return size_; }
  /// @}

  /**
   * \name max_size
   * \brief returns the maximum number of characters
   */
  constexpr size_type max_size() const noexcept { return static_cast<size_type>(-1) / sizeof(value_type); }

  /**
   * \name empty
   * \brief checks whether the view is empty
   */
  constexpr bool empty() const noexcept { return size() == 0; }

  /**
   * \name remove_prefix
   * \brief shrinks the view by moving its start forward
   */
  constexpr void remove_prefix(size_type n) {
    data_ += n;
    size_ -= n;
  }

  /**
   * \name remove_suffix
   * \brief shrinks the view by moving its end backward
   */
  constexpr void remove_suffix(size_type n) {
    size_ -= n;
  }

  /**
   * \name swap
   * @{
   * \brief swaps the contents
   */
  constexpr void swap(basic_string_view& other) noexcept {
    const_pointer p = data_;
    data_ = other.data_;
    other.data_ = p;

    size_type s = size_;
    size_ = other.size_;
    other.size_ = s;
  }
  /// @}

  /**
   * \name copy
   * @{
   * \brief copies characters
   */
  constexpr size_type copy(value_type* dest, size_type count, size_type pos = 0) const {
    if (pos > size()) {
      throw std::out_of_range("vccc::string_view::copy : out of range");
    }
    return traits_type::copy(dest, data() + pos, (std::min)(count, size() - pos));
  }
  /// @}

  /**
   * \name substr
   * \brief returns a substring
   * @{
   */
  constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const {
    if (pos > size()) {
      throw std::out_of_range("vccc::string_view::substr : out of range");
    }
    return basic_string_view(data() + pos, (std::min)(count, size() - pos));
  }
  /// @}

  /**
   * \name compare
   * \brief compares two views
   * @{
   */
  constexpr int compare(basic_string_view other) const noexcept {
    int r = traits_type::compare(data(), other.data(), (std::min)(size(), other.size()));
    if (r == 0) {
      return size() < other.size() ? -1 :
             size() > other.size() ?  1 :
             0;
    }
    return r;
  }

  constexpr int compare(size_type pos1, size_type count1, basic_string_view other) const {
    return substr(pos1, count1).compare(other);
  }

  constexpr int compare(size_type pos1, size_type count1, basic_string_view other, size_type pos2, size_type count2) const {
    return substr(pos1, count1).compare(other.substr(pos2, count2));
  }

  constexpr int compare(const CharT* s) const {
    return compare(basic_string_view(s));
  }

  constexpr int compare(size_type pos1, size_type count1, const value_type* s) const {
    return substr(pos1, count1).compare(basic_string_view(s));
  }

  constexpr int compare(size_type pos1, size_type count1, const value_type* s, size_type count2) const {
    return substr(pos1, count1).compare(basic_string_view(s, count2));
  }
  /// @}

  /**
   * \name starts_with
   * @{
   * \brief checks if the string view starts with the given prefix
   */
  constexpr bool starts_with(basic_string_view prefix) const noexcept {
    return basic_string_view(data(), (std::min)(size(), prefix.size())) == prefix;
  }

  constexpr bool starts_with(value_type c) const noexcept {
    return !empty() && traits_type::eq(front(), c);
  }

  constexpr bool starts_with(const value_type* s) const {
    return starts_with(basic_string_view(s));
  }
  /// @}

  /**
   * \name ends_with
   * @{
   * \brief checks if the string view ends with the given suffix
   */
  constexpr bool ends_with(basic_string_view sv) const noexcept {
    return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0;
  }

  constexpr bool ends_with(value_type c) const noexcept {
    return !empty() && traits_type::eq(back(), c);
  }

  constexpr bool ends_with(const value_type* s) const {
    return ends_with(basic_string_view(s));
  }
  /// @}


  /**
   * \name contains
   * @{
   * \brief checks if the string view contains the given substring or character
   */
  constexpr bool contains(basic_string_view sv) const noexcept {
    return find(sv) != npos;
  }

  constexpr bool contains(value_type c) const noexcept {
    return find(c) != npos;
  }

  constexpr bool contains(const value_type* str) const {
    return find(str) != npos;
  }
  /// @}

  /**
   * \name find
   * @{
   * \brief find characters in the view
   */
  constexpr size_type find(basic_string_view sv, size_type pos = 0) const noexcept {
    while (pos <= size() - sv.size()) {
      if (traits_type::compare(data() + pos, sv.data(), sv.size()) == 0)
        return pos;
      ++pos;
    }
    return npos;
  }

  constexpr size_type find(value_type c, size_type pos = 0) const noexcept {
    return find(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type find(const value_type* str, size_type pos, size_type count) const {
    return find(basic_string_view(str, count), pos);
  }

  constexpr size_type find(const value_type* str, size_type pos = 0) const {
    return find(basic_string_view(str), pos);
  }
  /// @}

  /**
   * \name rfind
   * \brief find the last occurrence of a substring
   * @{
   */
  constexpr size_type rfind(basic_string_view sv, size_type pos = npos) const noexcept {
    pos = (std::min)(size() - sv.size(), pos);
    while (pos <= size() - sv.size()) {
      if (traits_type::compare(data() + pos, sv.data(), sv.size()) == 0)
        return pos;
      --pos;
    }
    return npos;
  }

  constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept {
    return rfind(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type rfind(const value_type* str, size_type pos, size_type count) const {
    return rfind(basic_string_view(str, count), pos);
  }

  constexpr size_type rfind(const value_type* str, size_type pos = npos) const {
    return rfind(basic_string_view(str), pos);
  }
  /// @}

  /**
   * \name find_first_of
   * @{
   * \brief find first occurrence of characters
   */
  constexpr size_type find_first_of(basic_string_view sv, size_type pos = 0) const noexcept {
    while (pos < size()) {
      if (traits_type::find(sv.data(), sv.size(), (*this)[pos])) {
        return pos;
      }
      ++pos;
    }
    return npos;
  }

  constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_of(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type find_first_of(const value_type* s, size_type pos, size_type count) const {
    return find_first_of(basic_string_view(s, count), pos);
  }

  constexpr size_type find_first_of(const value_type* s, size_type pos = 0) const {
    return find_first_of(basic_string_view(s), pos);
  }
  /// @}

  /**
   * \name find_last_of
   * @{
   * \brief find last occurrence of characters
   */
  constexpr size_type find_last_of(basic_string_view sv, size_type pos = npos) const noexcept {
    pos = (std::min)(size() - 1, pos);
    while (pos < size()) {
      if (traits_type::find(sv.data(), sv.size(), (*this)[pos])) {
        return pos;
      }
      --pos;
    }
    return npos;
  }

  constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_of(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type find_last_of(const value_type* s, size_type pos, size_type count) const {
    return find_last_of(basic_string_view(s, count), pos);
  }

  constexpr size_type find_last_of(const value_type* s, size_type pos = npos) const {
    return find_last_of(basic_string_view(s), pos);
  }
  /// @}

  /**
   * \name find_first_not_of
   * @{
   * \brief find first absence of characters
  */
  constexpr size_type find_first_not_of(basic_string_view sv, size_type pos = 0) const noexcept {
    while (pos < size()) {
      if (!traits_type::find(sv.data(), sv.size(), (*this)[pos])) {
        return pos;
      }
      ++pos;
    }
    return npos;
  }

  constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_not_of(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type find_first_not_of(const value_type* s, size_type pos, size_type count) const {
    return find_first_not_of(basic_string_view(s, count), pos);
  }

  constexpr size_type find_first_not_of(const value_type* s, size_type pos = 0) const {
    return find_first_not_of(basic_string_view(s), pos);
  }
  /// @}

  /**
   * \name find_last_not_of
   * @{
   * \brief find last absence of characters
   */
  constexpr size_type find_last_not_of(basic_string_view sv, size_type pos = npos) const noexcept {
    pos = (std::min)(size() - 1, pos);
    while (pos < size()) {
      if (!traits_type::find(sv.data(), sv.size(), (*this)[pos])) {
        return pos;
      }
      --pos;
    }
    return npos;
  }

  constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_not_of(basic_string_view(vccc::addressof(c), 1), pos);
  }

  constexpr size_type find_last_not_of(const value_type* s, size_type pos, size_type count) const {
    return find_last_not_of(basic_string_view(s, count), pos);
  }

  constexpr size_type find_last_not_of(const value_type* s, size_type pos = npos) const {
    return find_last_not_of(basic_string_view(s), pos);
  }
  /// @}

 private:
  const_pointer data_;
  size_type size_;
};

// Extra template parameters are used to workaround a issue in MSVC's ABI (name decoration) (VSO-409326)

// operator==
template<typename CharT, typename Traits>
constexpr bool operator==(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator==(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator==(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
}
/// @endcond

// operator!=
template<typename CharT, typename Traits>
constexpr bool operator!=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return !(lhs == rhs);
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator!=(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return !(lhs == rhs);
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator!=(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return !(lhs == rhs);
}
/// @endcond

// operator<
template<typename CharT, typename Traits>
constexpr bool operator<(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator<(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator<(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}
/// @endcond

// operator<=
template<typename CharT, typename Traits>
constexpr bool operator<=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator<=(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator<=(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}
/// @endcond

// operator>
template<typename CharT, typename Traits>
constexpr bool operator>(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) > 0;
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator>(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) > 0;
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator>(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return lhs.compare(rhs) > 0;
}
/// @endcond

// operator>=
template<typename CharT, typename Traits>
constexpr bool operator>=(basic_string_view<CharT, Traits> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}

/// @cond ignored
template<typename CharT, typename Traits, int = 1>
constexpr bool operator>=(type_identity_t<basic_string_view<CharT, Traits>> lhs, basic_string_view<CharT, Traits> rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}

template<typename CharT, typename Traits, int = 2>
constexpr bool operator>=(basic_string_view<CharT, Traits> lhs, type_identity_t<basic_string_view<CharT, Traits>> rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}
/// @endcond

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, basic_string_view<CharT, Traits> sv) {
  using ostream_type = std::basic_ostream<CharT, Traits>;
  using iostate_type = typename ostream_type::iostate;
  using sentry_type = typename ostream_type::sentry;
  using size_type = std::size_t;

  iostate_type state = ostream_type::goodbit;
  sentry_type sentry(os);

  if (!sentry) {
    state |= ostream_type::badbit;
  } else {
    size_type pad;
    if (os.width() <= 0 || static_cast<size_type>(os.width()) <= sv.size()) {
      pad = 0;
    } else {
      pad = static_cast<size_type>(os.width()) - sv.size();
    }

    try {
      // pad on left
      if ((os.flags() & ostream_type::adjustfield) != ostream_type::left) {
        for (; 0 < pad; --pad) {
          if (Traits::eq_int_type(Traits::eof(), os.rdbuf()->sputc(os.fill()))) {
            state |= ostream_type::badbit; // insertion failed, quit
            break;
          }
        }
      }

      std::streamsize n = (std::max)(os.width(), static_cast<std::streamsize>(sv.size()));
      if (state == ostream_type::goodbit && os.rdbuf()->sputn(sv.data(), n) != n) {
        state |= ostream_type::badbit;
      }

      // pad on right
      if (state == ostream_type::goodbit) {
        for (; 0 < pad; --pad) {
          if (Traits::eq_int_type(Traits::eof(), os.rdbuf()->sputc(os.fill()))) {
            state |= ostream_type::badbit; // insertion failed, quit
            break;
          }
        }
      }

      os.width(0);
    } catch (...) {
      if ((os.exceptions() & ostream_type::badbit) != 0) {
        std::rethrow_exception(std::current_exception());
      }
    }
  }

  os.setstate(state);
  return os;
}

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;
#if __cplusplus >= 202002L && VCCC_HAS_TYPE_CHAR8_T_CXX20
using u8string_view = basic_string_view<char8_t>;
#endif
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

inline namespace literals {
inline namespace string_view_literals {

constexpr string_view operator ""_sv(const char* str, std::size_t len) noexcept {
  return string_view{str, len};
}

#if __cplusplus >= 202002L && VCCC_HAS_TYPE_CHAR8_T_CXX20
constexpr u8string_view operator ""_sv(const char8_t* str, std::size_t len) noexcept {
  return u8string_view{str, len};
}
#endif

constexpr u16string_view operator ""_sv(const char16_t* str, std::size_t len) noexcept {
  return u16string_view{str, len};
}

constexpr u32string_view operator ""_sv(const char32_t* str, std::size_t len) noexcept {
  return u32string_view{str, len};
}

constexpr wstring_view operator ""_sv(const wchar_t* str, std::size_t len) noexcept {
  return wstring_view{str, len};
}

} // namespace string_view_literals
} // namespace literals

template<typename CharT, typename Traits>
struct ranges::enable_borrowed_range<basic_string_view<CharT, Traits>>
    : std::true_type {};

template<typename CharT, typename Traits >
struct ranges::enable_view<basic_string_view<CharT, Traits>> : std::true_type {};

#if __cplusplus >= 201703L
template<typename It, typename End>
basic_string_view(It, End) -> basic_string_view<iter_value_t<It>>;

template<typename R>
basic_string_view(R&&) -> basic_string_view<ranges::range_value_t<R>>;

#endif

/// @} string_view

} // namespace vccc


namespace std {

//! @addtogroup string_view
//! @{

template<>
struct hash<vccc::string_view> {
  std::size_t operator()(const vccc::string_view& sv) const noexcept {
    return vccc::hash_array(sv.data(), sv.size());
  }
};

template<>
struct hash<vccc::wstring_view> {
  std::size_t operator()(const vccc::wstring_view& sv) const noexcept {
    return vccc::hash_array(sv.data(), sv.size());
  }
};

#if __cplusplus >= 202002L && VCCC_HAS_TYPE_CHAR8_T_CXX20
template<>
struct hash<vccc::u8string_view> {
  std::size_t operator()(const vccc::u8string_view& sv) const noexcept {
    return vccc::hash_array(sv.data(), sv.size());
  }
};
#endif

template<>
struct hash<vccc::u16string_view> {
  std::size_t operator()(const vccc::u16string_view& sv) const noexcept {
    return vccc::hash_array(sv.data(), sv.size());
  }
};

template<>
struct hash<vccc::u32string_view> {
  std::size_t operator()(const vccc::u32string_view& sv) const noexcept {
    return vccc::hash_array(sv.data(), sv.size());
  }
};

/// @} string_view

} // namespace std

#endif // VCCC_STRING_VIEW_HPP_
