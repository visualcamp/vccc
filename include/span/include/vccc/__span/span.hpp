//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_SPAN_SPAN_HPP
#define VCCC_SPAN_SPAN_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__memory/to_address.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/enable_view.hpp"
#include "vccc/__ranges/borrowed_range.hpp"
#include "vccc/__ranges/contiguous_range.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__span/dynamic_extent.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__type_traits/type_identity.hpp"

namespace vccc {

template<typename T, std::size_t Extent = dynamic_extent>
class span;

namespace detail {

template<typename T, std::size_t Extent>
struct static_span_storage {
  constexpr static_span_storage() noexcept : ptr_(nullptr) {}
  constexpr static_span_storage(T* ptr, std::size_t) noexcept : ptr_(ptr) {}

  T* data() const noexcept { return ptr_; }
  std::size_t size() const noexcept { return Extent; }

  T* ptr_;
};

template<typename T, std::size_t>
struct dynamic_span_storage {
  constexpr dynamic_span_storage() noexcept : ptr_(nullptr), size_(0) {}
  constexpr dynamic_span_storage(T* ptr, std::size_t size) noexcept : ptr_(ptr), size_(size) {}

  T* data() const noexcept { return ptr_; }
  std::size_t size() const noexcept { return size_; }

  T* ptr_;
  std::size_t size_;
};

template<typename T, std::size_t Extent>
using span_storage_t = std::conditional_t<
    Extent == dynamic_extent, dynamic_span_storage<T, Extent>,
    static_span_storage<T, Extent>>;

template<typename T>
struct is_span : std::false_type {};
template<typename T, std::size_t Extent>
struct is_span<span<T, Extent>> : std::true_type {};

template<typename T>
struct is_array : std::false_type {};
template<typename T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};

template<typename It, typename T, bool = contiguous_iterator<It>::value /* true */>
struct span_ctor_first_count : std::is_convertible<iter_reference_t<It>, T> {};
template<typename It, typename T>
struct span_ctor_first_count<It, T, false> : std::false_type {};

template<typename It, typename End, typename T, bool = conjunction<
    contiguous_iterator<It>, sized_sentinel_for<End, It> >::value /* true */>
struct span_ctor_first_last : conjunction<
    std::is_convertible<iter_reference_t<It>, T>,
    negation<std::is_convertible<End, std::size_t>> > {};
template<typename It, typename End, typename T>
struct span_ctor_first_last<It, End, T, false> : std::false_type {};

template<typename R, typename T, bool = conjunction<
      ranges::contiguous_range<R>,
      ranges::sized_range<R>,
      disjunction< ranges::borrowed_range<R>, std::is_const<T> >,
      negation< is_span<remove_cvref_t<R>> >,
      negation< is_array<remove_cvref_t<R>> >,
      negation< std::is_array<remove_cvref_t<R>> >
    >::value /* true */>
struct span_ctor_range : std::is_convertible<ranges::range_reference_t<R>, T> {};
template<typename R, typename T>
struct span_ctor_range<R, T, false> : std::false_type {};

} // namespace detail

/// @addtogroup span
/// @{

/**
@brief a non-owning view over a contiguous sequence of objects

The class template `%span` describes an object that can refer to a contiguous sequence of objects with the first element
of the sequence at position zero. A span can either have a \a static extent, in which case the number of elements in the
sequence is known at compile-time and encoded in the type, or a \a dynamic extent.

If a span has \a dynamic extent, a typical implementation holds two members: a pointer to `T` and a size. A span with
\a static extent may have only one member: a pointer to `T`.

@sa [std::span](https://en.cppreference.com/w/cpp/container/span)
 */
template<typename T, std::size_t Extent>
class span : private detail::span_storage_t<T, Extent> {
 public:
  using element_type = T;
  using value_type = std::remove_cv_t<T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using iterator = pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;

 private:
  using base = detail::span_storage_t<T, Extent>;

  template<std::size_t Offset, std::size_t Count>
  using subspan_type = span<element_type,
                            (Count != dynamic_extent ? Count :
                             Extent != dynamic_extent ? Extent - Offset :
                             dynamic_extent)>;
 public:
  // (1)
  // constexpr span() noexcept;
  constexpr span() noexcept : base() {}

  // (2)
  // template< class It >
  // explicit(extent != std::dynamic_extent)
  // constexpr span( It first, size_type count );
  template<typename It, std::enable_if_t<
    conjunction<
      bool_constant<(Extent == dynamic_extent)>,
      detail::span_ctor_first_count<It, element_type>
    >::value, int> = 0>
  constexpr span(It first, size_type size)
      : base(vccc::to_address(first), size) {}

  template<typename It, std::enable_if_t<
    conjunction<
      bool_constant<(Extent != dynamic_extent)>,
      detail::span_ctor_first_count<It, element_type>
    >::value, int> = 0>
  constexpr explicit span(It first, size_type size)
      : base(vccc::to_address(first), size) {}

  // (3)
  // template< class It, class End >
  // explicit(extent != std::dynamic_extent)
  // constexpr span( It first, End last );
  template<typename It, typename End, std::enable_if_t<
    conjunction<
      bool_constant<(Extent == dynamic_extent)>,
      detail::span_ctor_first_last<It, End, element_type>
    >::value , int> = 0>
  constexpr span(It first, End last)
      : base(vccc::to_address(first), last - first) {}

  template<typename It, typename End, std::enable_if_t<
    conjunction<
      bool_constant<(Extent != dynamic_extent)>,
      detail::span_ctor_first_last<It, End, element_type>
    >::value , int> = 0>
  constexpr explicit span(It first, End last)
      : base(vccc::to_address(first), last - first) {}

  // (4)
  // template< std::size_t N >
  // constexpr span( std::type_identity_t<element_type> (&arr)[N] ) noexcept;
  template<std::size_t N, std::enable_if_t<
    conjunction<
      bool_constant<(Extent == dynamic_extent) || (Extent == N)>,
      std::is_convertible<type_identity_t<element_type>, element_type>
    >::value, int> = 0>
  constexpr span(type_identity_t<element_type> (&arr)[N]) noexcept
      : base(arr, N) {}

  // (5)
  // template< class U, std::size_t N >
  // constexpr span( std::array<U, N>& arr ) noexcept;
  template<typename U, std::size_t N, std::enable_if_t<
    conjunction<
      bool_constant<(Extent == dynamic_extent) || (Extent == N)>,
      std::is_convertible<U, element_type>
    >::value, int> = 0>
  constexpr span(std::array<U, N>& arr) noexcept
      : base(arr.data(), N) {}

  // (6)
  // template< class U, std::size_t N >
  // constexpr span( const std::array<U, N>& arr ) noexcept;
  template<typename U, std::size_t N, std::enable_if_t<
    conjunction<
      bool_constant<(Extent == dynamic_extent) || (Extent == N)>,
      std::is_convertible<const U, element_type>
    >::value, int> = 0>
  constexpr span(const std::array<U, N>& arr) noexcept
      : base(arr.data(), N) {}

  // (7)
  // template< class R >
  // explicit(extent != std::dynamic_extent)
  // constexpr span( R&& range );
  template<typename R, std::enable_if_t<
    conjunction<
      bool_constant<Extent == dynamic_extent>,
      detail::span_ctor_range<R, element_type>
  >::value, int> = 0>
  constexpr span(R&& range)
      : base(ranges::data(std::forward<R>(range)),
             ranges::size(std::forward<R>(range))) {}

  template<typename R, std::enable_if_t<
    conjunction<
      bool_constant<Extent != dynamic_extent>,
      detail::span_ctor_range<R, element_type>
  >::value, int> = 0>
  constexpr explicit span(R&& range)
      : base(ranges::data(std::forward<R>(range)),
             ranges::size(std::forward<R>(range))) {}

  // (8) (C++ 26)
  // explicit(extent != std::dynamic_extent)
  // constexpr span( std::initializer_list<value_type> il ) noexcept;

  // (9)
  // template< class U, std::size_t N >
  // explicit(extent != std::dynamic_extent && N == std::dynamic_extent)
  // constexpr span( const std::span<U, N>& source ) noexcept;
  template<typename U, std::size_t N, std::enable_if_t<
      conjunction<
        bool_constant<!(Extent != dynamic_extent && N == dynamic_extent)>,
        bool_constant<(
          Extent == dynamic_extent ||
          N == dynamic_extent ||
          N == Extent)>,
        std::is_convertible<U, element_type>
      >::value, int> = 0>
  constexpr span(const span<U, N>& source) noexcept : base(source.data(), source.size()) {}

  template<typename U, std::size_t N, std::enable_if_t<
      conjunction<
        bool_constant<(Extent != dynamic_extent && N == dynamic_extent)>,
        bool_constant<(
          Extent == dynamic_extent ||
          N == dynamic_extent ||
          N == Extent)>,
        std::is_convertible<U, element_type>
      >::value, int> = 0>
  constexpr explicit span(const span<U, N>& source) noexcept : base(source.data(), source.size()) {}

  // (10)
  // constexpr span( const span& other ) noexcept = default;
  constexpr span(const span& other) noexcept = default;

  constexpr iterator begin() const noexcept { return iterator(data()); }
  constexpr iterator end() const noexcept { return iterator(data() + size()); }
  constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(data() + size()); }
  constexpr reverse_iterator rend() const noexcept { return reverse_iterator(data()); }

  constexpr reference front() const { return *begin(); }
  constexpr reference back() const { return *(end() - 1); }

  constexpr reference operator[](size_type idx) const { return data()[idx]; }

  constexpr pointer data() const noexcept { return base::data(); }

  constexpr size_type size() const noexcept { return base::size(); }
  constexpr size_type size_bytes() const noexcept { return size() * sizeof(element_type); }

  constexpr bool empty() const noexcept { return size() == 0; }

  template<std::size_t Count, std::enable_if_t<(Count <= Extent), int> = 0>
  constexpr span<element_type, Count> first() const {
    return span<element_type, Count>(data(), Count);
  }
  constexpr span<element_type, dynamic_extent> first(size_type Count) const {
    return span<element_type, dynamic_extent>(data(), Count);
  }

  template<std::size_t Count, std::enable_if_t<(Count <= Extent), int> = 0>
  constexpr span<element_type, Count> last() const {
    return span<element_type, Count>(data() + size() - Count, Count);
  }
  constexpr span<element_type, dynamic_extent> last(size_type Count) const {
    return span<element_type, dynamic_extent>(data() + size() - Count, Count);
  }

  template<std::size_t Offset, std::size_t Count = dynamic_extent, std::enable_if_t<
    (Offset <= Extent) && (Count == dynamic_extent || Count <= (Extent - Offset)), int> = 0>
  constexpr subspan_type<Offset, Count> subspan() const {
    return subspan_type<Offset, Count>(
      data() + Offset,
      (Count == dynamic_extent ? size() - Offset : Count));
  };
  constexpr span<element_type, dynamic_extent> subspan(size_type Offset, size_type Count) const {
    return span<element_type, dynamic_extent>(
      data() + Offset,
      (Count == dynamic_extent ? size() - Offset : Count));
  }
};

template<typename T, std::size_t Extent>
struct ranges::enable_borrowed_range<span<T, Extent>> : std::true_type {};

template<typename T, std::size_t Extent>
struct ranges::enable_view<span<T, Extent>> : std::true_type {};

#if __cplusplus >= 201703L

template<typename It, typename EndOrSize, std::enable_if_t<contiguous_iterator<It>::value, int> = 0>
span(It, EndOrSize) -> span<std::remove_reference_t<iter_reference_t<It>>>;

template<typename T, std::size_t N>
span(T (&)[N]) -> span<T, N>;

template<typename T, std::size_t N>
span(std::array<T, N>&) -> span<T, N>;

template<typename T, std::size_t N>
span(const std::array<T, N>&) -> span<const T, N>;

template<typename R>
span(R&&) -> span<std::remove_reference_t<ranges::range_reference_t<R>>>;

#endif

/// @}

} // namespace vccc

#endif // VCCC_SPAN_SPAN_HPP
