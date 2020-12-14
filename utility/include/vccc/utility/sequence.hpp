# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_UTILITY_SEQUENCE_HPP
# define VCCC_UTILITY_SEQUENCE_HPP
#
# include "vccc/utility/detail/custom_sequence.hpp"

namespace vccc {

template <std::size_t N>
using make_rindex_sequence
= decltype(detail::rindex_sequence(std::make_index_sequence<N>{}));

template <typename T, T v, std::size_t N>
using make_same_sequence
= decltype(detail::same_sequence<T, v>(std::make_integer_sequence<T, N>{}));

template <std::size_t N>
using make_zero_sequence = make_same_sequence<int, 0, N>;


// creates a sequence that has begin, begin + step, begin + 2*step, ... that satisfies [begin, end)
template <typename T, T begin, T end, T step=1, T size = (end + (end>0?-1:1) - begin)/step>
using make_range_sequence
= decltype(detail::range_sequence<T, begin, step>(std::make_integer_sequence<T, (size >= 0 ? size+1 : size)>{}));

template <std::size_t begin, std::size_t end, std::size_t step=1, std::size_t size = (end + (end>0?-1:1) - begin)/step>
using make_index_range_sequence
= decltype(detail::range_sequence<std::size_t, begin, step>(std::make_integer_sequence<std::size_t, (size >= 0 ? size+1 : size)>{}));

}

# endif //VCCC_UTILITY_SEQUENCE_HPP
