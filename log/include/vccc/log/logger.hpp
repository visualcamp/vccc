# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_LOG_LOGGER_HPP
# define VCCC_LOG_LOGGER_HPP
#
# include <vector>
# include <sstream>
# include <string>
# include <regex>
# include "vccc/log/detail/c_printable.hpp"
# include "vccc/log/detail/custom_ostream.hpp"

namespace vccc{

using c_printable = std::true_type;
using not_c_printable = std::false_type;

class Logger {
 public:
  template<typename ...Args>
  Logger(const Args&... args);

  inline std::string get() const {
    return out.str();
  }

 private:
  template<typename CharT>
  static bool isFormatted(const CharT& str);

  template<typename Arg, typename ...Args>
  void addImpl(c_printable, const Arg& arg, const Args&... args);

  template<typename Arg, typename ...Args>
  void addImpl(not_c_printable, const Arg& arg, const Args&... args);

  template<typename Arg>
  void add(const Arg& n);

  template<typename Arg, typename ...Args>
  void add(const Arg& arg, const Args&... args);

  template<typename ...Args>
  void addFormatted(const char* fmt, const Args&... val);

  std::vector<char> buffer;
  std::stringstream out;
//  static std::string separator;
};

template<typename ...Args>
Logger::Logger(const Args& ...args)
{
  addImpl(detail::are_types_c_printable<Args...>(), args...);
}

// TODO: move fmt_reg to class scope
// TODO: add PRI.. format
template<typename CharT>
bool
Logger::isFormatted(const CharT& str)
{
  static const std::regex fmt_reg(R"(%(?:\d+\$)?[+-]?(?:[ 0]|'.{1})?-?\d*(?:\.\d+)?[bcdeEufFgGosxXp])");
  return std::regex_search(str, fmt_reg);
}

template<typename T>
void
Logger::add(const T& n)
{
  out << n;
}

template<typename Arg, typename ...Args>
void
Logger::add(const Arg& arg, const Args&... args)
{
  add(arg);
  add(' '); // default separator
  add(args...);
}

template<typename Arg, typename ...Args>
void
Logger::addImpl(c_printable, const Arg& arg, const Args&... args)
{
  if (isFormatted(arg))
    addFormatted(arg, args...);
  else
    addImpl(not_c_printable(), arg, args...);
}

template<typename Arg, typename ...Args>
void
Logger::addImpl(not_c_printable, const Arg& arg, const Args&... args)
{
  return add(arg, args...);
}

template<typename ...Args>
void
Logger::addFormatted(const char* fmt, const Args&... val)
{
  int size = snprintf(NULL, 0, fmt, val...);
  if(buffer.size() < size + 1)    buffer.resize(size + 1);
  else if (buffer.size() > 100)   buffer.resize(100); // prevents too frequent resizing
  snprintf(buffer.data(), size + 1, fmt, val...);
  out << buffer.data();
}

}

# endif //VCCC_LOG_LOGGER_HPP
