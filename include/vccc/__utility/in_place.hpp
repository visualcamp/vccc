# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_UTILITY_IN_PLACE_HPP
# define VCCC_UTILITY_IN_PLACE_HPP
#
# include <cstddef>
#if __cplusplus >= 201703L
# include <utility>
#endif
#
# include "vccc/__core/inline_or_static.hpp"

namespace vccc {

//! @addtogroup utility
//! @{

#if __cplusplus < 201703L

/**
 * @brief in-place construction tag
 */
struct in_place_t {
  constexpr explicit in_place_t() = default;
};

template<typename T>
struct in_place_type_t {
  constexpr explicit in_place_type_t() = default;
};

template<std::size_t I>
struct in_place_index_t {
  constexpr explicit in_place_index_t() = default;
};

#else

using in_place_t = std::in_place_t;
template<typename T> using in_place_type_t = std::in_place_type_t<T>;
template<std::size_t I> using in_place_index_t = std::in_place_index_t<I>;

#endif

VCCC_INLINE_OR_STATIC constexpr in_place_t in_place{};

template<typename T>
VCCC_INLINE_OR_STATIC constexpr in_place_type_t<T> in_place_type{};

template<std::size_t I>
VCCC_INLINE_OR_STATIC constexpr in_place_index_t<I> in_place_index{};

//! @} optional

} // namespace vccc

# endif // VCCC_UTILITY_IN_PLACE_HPP
