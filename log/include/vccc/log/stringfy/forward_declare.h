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
# include <chrono>
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

//// stringfy

// chrono
template<typename Rep, typename Period>
std::string stringfy(const std::chrono::duration<Rep, Period>& duration);

template<typename Duration>
std::string stringfy(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point);

template<typename Clock, typename Duration>
std::string stringfy(const std::chrono::time_point<Clock, Duration>& time_point);

std::string stringfy(const std::time_t* tt);


// container
template<typename T, VCCC_ENABLE_IF_FORWARD(is_container_v<T>)> std::string stringfy(const T& value);


// tuple-like
template<typename ...Ts> std::string stringfy(const std::tuple<Ts...>& value);
template<typename T1, typename T2> std::string stringfy(const std::pair<T1, T2>& value);


// etc
template<typename T, T ...I> std::string stringfy(const std::integer_sequence<T, I...>&);


template<typename T, typename = void>
struct support_stringfy : public std::false_type {};

template<typename T>
struct support_stringfy<T, ::vccc::void_t<decltype(stringfy(std::declval<T>()))>> : public std::true_type {};

template<typename T>
using support_stringfy_t = typename support_stringfy<T>::type;

template<typename T>
constexpr bool support_stringfy_v = support_stringfy<T>::value;



namespace internal {
using default_printable = std::true_type;
using not_default_printable = std::false_type;

template<typename T> std::ostream& write_to_stream_impl(default_printable, std::ostream& os, const T& value) {
  os << value;
  return os;
}

template<typename T> std::ostream& write_to_stream_impl(not_default_printable, std::ostream& os, const T& value) {
  static_assert(support_stringfy_v<T>,
      "This type does not support operator<<(std::ostream&, const T&), and vccc::stringfy(const T&)."
      "Define operator<<(std::ostream&, const T&) on your custom type T.");

  os << stringfy(value);
  return os;
}

} // namespace internal

template<typename T>
inline std::ostream&
write_to_stream(std::ostream& os, const T& value) {
  return internal::write_to_stream_impl(is_printable<T>{}, os, value);
}

} // namespace vccc

# endif //VCCC_LOG_STRINGFY_FORWARD_DECLARE_H_
