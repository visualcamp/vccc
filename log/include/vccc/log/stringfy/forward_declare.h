# /*
#  * Created by YongGyu Lee on 2021/03/12.
#  */
#
# ifndef VCCC_LOG_STRINGFY_FORWARD_DECLARE_H_
# define VCCC_LOG_STRINGFY_FORWARD_DECLARE_H_
#
# include <ostream>
# include <tuple>
# include <utility>
# include "vccc/type_traits.hpp"

namespace vccc {

namespace internal {

template<typename InputIterator>
std::string stringfy_iterator(InputIterator begin, InputIterator end);

template<typename Tuple, std::size_t ...I>
std::string stringfy_tuple(const Tuple& tup, std::index_sequence<I...>);

template<typename ...Args>
std::string stringfy_variadics(const Args&... args);

}

template<typename T, VCCC_ENABLE_IF_FORWARD(is_container_v<T>)> std::string stringfy(const T& value);
template<typename ...Ts> std::string stringfy(const std::tuple<Ts...>& value);
template<typename T1, typename T2> std::string stringfy(const std::pair<T1, T2>& value);
template<typename T, T ...I> std::string stringfy(const std::integer_sequence<T, I...>&);

namespace internal {
using default_printable = std::true_type;
using not_default_printable = std::false_type;

template<typename T> std::ostream& write_to_stream_impl(default_printable, std::ostream& os, const T& value);

template<typename T> std::ostream& write_to_stream_impl(not_default_printable, std::ostream& os, const T& value);

} // namespace internal

template<typename T>
std::ostream& write_to_stream(std::ostream& os, const T& value) {
  return internal::write_to_stream_impl(is_printable<T>{}, os, value);
}

} // namespace vccc

# endif //VCCC_LOG_STRINGFY_FORWARD_DECLARE_H_
