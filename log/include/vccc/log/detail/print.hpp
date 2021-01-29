# /*
#  * Created by YongGyu Lee on 2020/12/02.
#  */
#
# ifndef VCCC_LOG_DETAIL_PRINT_HPP
# define VCCC_LOG_DETAIL_PRINT_HPP
#
# include <iostream>
# include "vccc/type_traits.hpp"

namespace vccc{

template<typename ...Args> std::ostream& operator << (std::ostream& os, const std::tuple<Args...>& tup);
template<typename ...Args> std::ostream& operator << (std::ostream& os, const std::pair<Args...>& tup);
template<typename T, T... I> std::ostream& operator << (std::ostream& os, const std::integer_sequence<T, I...>& seq);
template<typename T, VCCC_REQUIRE_FORWARD(is_container<T>::value && !is_string_like<T>::value)>
std::ostream& operator << (std::ostream& os, const T& v);

namespace detail{

template<typename Iterator>
std::ostream&
print_iterator(std::ostream& os, Iterator first, Iterator last)
{
  os << '{';
  if (first != last) {
    os << ' ';
    os << *first;
    ++first;
    for(; first != last; ++first)
      os << ", " << *first;
    os << ' ';
  }
  os << '}';
  return os;
}

template<typename Tuple, std::size_t ...I>
std::ostream&
print_tuple(std::ostream& os, const Tuple& tup, std::index_sequence<0, I...>)
{
  os << "{ ";
  os << std::get<0>(tup);
  [[maybe_unused]]
  int dummy[sizeof...(I)] = {
      (os << ", " << std::get<I>(tup), 0)...
  };
  os << " }";
  return os;
}

namespace impl{
  template<typename Arg>
  void
  print_multiples_impl(std::ostream& os, const Arg& arg)
  {
    os << arg;
  }

  template<typename Arg, typename ...Args>
  void
  print_multiples_impl(std::ostream& os, const Arg& arg, const Args&... args)
  {
    print_multiples_impl(os, arg);
    print_multiples_impl(os, ", ");
    print_multiples_impl(os, args...);
  }

}

template<typename ...Args>
void
print_multiples(std::ostream& os, const Args&... args)
{
  os << '{';
  if(sizeof...(Args) > 0) {
    os << ' ';
    impl::print_multiples_impl(os, args...);
    os << ' ';
  }
  os << '}';
}


}}

# endif //VCCC_LOG_DETAIL_PRINT_HPP
