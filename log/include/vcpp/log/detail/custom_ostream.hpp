//
// Created by YongGyu Lee on 2020/12/02.
//

#ifndef VCPP_LOG_CUSTOM_OSTREAM_HPP
#define VCPP_LOG_CUSTOM_OSTREAM_HPP

#include "vcpp/log/detail/print.hpp"
#include "vcpp/type_traits.hpp"

namespace vc{

template<typename ...Args> std::ostream& operator << (std::ostream& os, const std::tuple<Args...>& tup);
template<typename ...Args> std::ostream& operator << (std::ostream& os, const std::pair<Args...>& tup);
template<typename T, T... I> std::ostream& operator << (std::ostream& os, const std::integer_sequence<T, I...>& seq);


template<typename T, VCPP_REQUIRE(is_container<T>::value && !is_printable<T>::value)>
std::ostream& operator << (std::ostream& os, const T& v){
  return detail::print_iterator(os, std::begin(v), std::end(v));
}

template<typename ...Args>
std::ostream& operator << (std::ostream& os, const std::tuple<Args...>& tup) {
  return detail::print_tuple(os, tup, std::index_sequence_for<Args...>{});
}

template<typename ...Args>
std::ostream& operator << (std::ostream& os, const std::pair<Args...>& tup) {
  return detail::print_tuple(os, tup, std::index_sequence_for<Args...>{});
}

template<typename T, T... I>
std::ostream& operator << (std::ostream& os, const std::integer_sequence<T, I...>& seq) {
  detail::print_multiples(os, I...);
  return os;
}


}

#endif //VCPP_LOG_CUSTOM_OSTREAM_HPP
