# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_UTILITY_SEQUENCE_HPP
# define VCCC_UTILITY_SEQUENCE_HPP
#
# include "vccc/utility/detail/custom_sequence.hpp"

namespace vccc {

//! @addtogroup utility_sequence
//! @{

/**
@brief alias of reverse index sequence

creates std::index_sequence<N-1, N-2, ... 0>
@tparam N sequence length

*/

template <std::size_t N>
using make_rindex_sequence
= decltype(internal::rindex_sequence(std::make_index_sequence<N>{}));

/**
@brief alias of same integer sequence

creates std::integer_sequence<T, v, v, ... v>
@tparam T type
@tparam v value
@tparam N sequence length

*/

template <typename T, T v, std::size_t N>
using make_same_sequence
= decltype(internal::same_sequence<T, v>(std::make_integer_sequence<T, N>{}));

/**
@brief alias of zero std::size_t sequence

creates std::index_sequence<0, 0, ... 0><br>
note that the type is std::size_t, not int
@tparam N sequence length

*/
template <std::size_t N>
using make_zero_sequence = make_same_sequence<int, 0, N>;


/**
@brief alias of range integer sequence

creates std::integer_sequence<T, begin, begin+step, ... >
template values satisfies [begin, end)

@tparam T type
@tparam begin begin value
@tparam end end value
@tparam step increment size(default = 1)
@tparam size deduced. change it at your own risk

*/
template <typename T, T begin, T end, T step=1, T size = (end + (end>0?-1:1) - begin)/step>
using make_range_sequence
= decltype(internal::range_sequence<T, begin, step>(std::make_integer_sequence<T, (size >= 0 ? size+1 : size)>{}));

/**
@brief alias of range index sequence
@sa make_range_sequence

@tparam begin begin value
@tparam end end value
@tparam step increment size(default = 1)
@tparam size deduced. change it at your own risk

*/
template <std::size_t begin, std::size_t end, std::size_t step=1, std::size_t size = (end + (end>0?-1:1) - begin)/step>
using make_index_range_sequence
= decltype(internal::range_sequence<std::size_t, begin, step>(std::make_integer_sequence<std::size_t, (size >= 0 ? size+1 : size)>{}));

//! @} utility utility_sequence

} // namespace vccc

# endif // VCCC_UTILITY_SEQUENCE_HPP
