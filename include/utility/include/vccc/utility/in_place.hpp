# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_UTILITY_IN_PLACE_HPP
# define VCCC_UTILITY_IN_PLACE_HPP
#
# include <cstddef>
#
# include "vccc/core/inline_or_static.hpp"

namespace vccc {

//! @addtogroup utility
//! @{

/**
 * @brief in-place construction tag
 */
struct in_place_t {
  constexpr explicit in_place_t() = default;
};

VCCC_INLINE_OR_STATIC constexpr in_place_t in_place{};

template<typename T>
struct in_place_type_t {
  constexpr explicit in_place_type_t() = default;
};

template<typename T>
VCCC_INLINE_OR_STATIC constexpr in_place_type_t<T> in_place_type{};

template<std::size_t I>
struct in_place_index_t {
  constexpr explicit in_place_index_t() = default;
};

template<std::size_t I>
VCCC_INLINE_OR_STATIC constexpr in_place_index_t<I> in_place_index{};

//! @} optional

} // namespace vccc

# endif // VCCC_UTILITY_IN_PLACE_HPP
