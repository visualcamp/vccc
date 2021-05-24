# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LOG_LOGGER_HPP
# define VCCC_LOG_LOGGER_HPP
#
# include <vector>
# include <sstream>
# include <string>
# include <regex>
# include "vccc/log/detail/c_printable.hpp"
# include "vccc/log/stream_wrapper.hpp"

namespace vccc{

//! @addtogroup log
//! @{


/**
@brief stream wrapper that can accept both C-style formatted and C++ style variadic

@code{.cpp}
    std::string str1 = Logger("%s %d", "Hello", 100).get();
    std::string str2 = Logger(1, "Hello", "world").get();
    std::string str3 = Logger(std::boolalpha, true).get();
@endcode

 */
class Logger {
 public:
  using c_printable = std::true_type;
  using not_c_printable = std::false_type;

  template<typename ...Args>
  Logger(const Args&... args) {
    addImpl(detail::are_types_c_printable_t<Args...>{}, args...);
  }

  inline std::string get() const {
    return out.stream().str();
  }
 private:

  template<typename CharT>
  static bool isFormatted(const CharT& str) {
    // TODO: move fmt_reg to class scope
    // TODO: add PRI.. format
    static const std::regex fmt_reg(R"(%(?:\d+\$)?[+-]?(?:[ 0]|'.{1})?-?\d*(?:\.\d+)?[bcdeEufFgGosxXp])");
    return std::regex_search(str, fmt_reg);
  }

  template<typename Arg, typename ...Args>
  void addImpl(c_printable, const Arg& arg, const Args&... args) {
    if (isFormatted(arg))
      addFormatted(arg, args...);
    else
      addImpl(not_c_printable(), arg, args...);
  }

  template<typename Arg, typename ...Args>
  inline void addImpl(not_c_printable, const Arg& arg, const Args&... args) {
    add(arg, args...);
  }

  template<typename Arg>
  inline void add(const Arg& n) {
    out << n;
  }

  template<typename Arg, typename ...Args>
  void add(const Arg& arg, const Args&... args) {
    add(arg);
    add(' '); // default separator
    add(args...);
  }

  template<typename ...Args>
  void addFormatted(const char* fmt, const Args&... val) {
    int size = snprintf(NULL, 0, fmt, val...);
    if(buffer.size() < size + 1)    buffer.resize(size + 1);
    else if (buffer.size() > 100)   buffer.resize(100); // prevents too frequent resizing
    snprintf(buffer.data(), size + 1, fmt, val...);
    out << buffer.data();
  }

  std::vector<char> buffer;
  StreamWrapper<std::stringstream> out;
};

//! @} log

}

# endif //VCCC_LOG_LOGGER_HPP
