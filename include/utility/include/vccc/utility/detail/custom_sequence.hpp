# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_UTILITY_DETAIL_CUSTOM_SEQUENCE_HPP
# define VCCC_UTILITY_DETAIL_CUSTOM_SEQUENCE_HPP
#
# include <utility>

namespace vccc {
namespace internal {

//! @cond IGNORED

template <typename T, T v, T... Is>
constexpr auto same_sequence (std::integer_sequence<T, Is...> const &)
-> decltype( std::integer_sequence<T, v + 0 * Is...>{} );

template <std::size_t ... Is>
constexpr auto rindex_sequence (std::index_sequence<Is...> const &)
-> decltype( std::index_sequence<sizeof...(Is) - 1U - Is...>{} );

template<typename T, T start, T incr, T... I>
constexpr auto range_sequence(std::integer_sequence<T,I...> const &)
-> decltype(std::integer_sequence<T, start + incr * I...>{});

//! @endcond

} // namespace internal
} // namespace vccc

# endif // VCCC_UTILITY_DETAIL_CUSTOM_SEQUENCE_HPP
