# /*
#  * Created by YongGyu Lee on 2021/03/20.
#  */
#
# ifndef VCCC_LOG_STREAM_WRAPPER_HPP_
# define VCCC_LOG_STREAM_WRAPPER_HPP_
#
# include <iostream>
# include <type_traits>
# include <iomanip>
# include <chrono>
# include <tuple>
# include <utility>
# include <string>
# include <functional>
# include "vccc/type_traits.hpp"

namespace vccc {

template<typename Stream>
class StreamWrapper {
 public:
  using stream_type = Stream;
  using char_type = typename stream_type::char_type;
  using traits_type = typename stream_type::traits_type;
  using basic_ostream_type = std::basic_ostream<char_type, traits_type>;


  // for std::endl
  inline StreamWrapper&
  operator << (std::ostream& (*pf)(std::ostream&)) { pf(stream_); return *this; }

  // default printable type
  template<typename T>
  inline StreamWrapper&
  operator << (const T& value) { try_write(typename is_printable<T>::type{}, value); return *this; }

  inline stream_type& stream() { return stream_; }
  inline const stream_type& stream() const { return stream_; }

 private:
  stream_type stream_;

  using default_printable_t = std::true_type;
  using not_default_printable_t = std::false_type;

  bool use_key_value_separator = true;

  using key_value_container_t = std::true_type;
  using not_key_value_container_t = std::false_type;

  template<typename T>
  using is_key_value_container_t
    = typename is_specialization<std::decay_t<decltype(*std::declval<T>().begin())>, std::pair>::type;

  template<typename T>
  inline void try_write(default_printable_t, const T& value) { stream_ << value;}

  template<typename T>
  inline void try_write(not_default_printable_t, const T& value) { write(value); }


  // container types
  template<typename T, VCCC_ENABLE_IF_FORWARD(is_container_v<T>)>
  void write(const T& value);

  template<typename T>
  void writeContainer(key_value_container_t, const T& value);

  template<typename T>
  void writeContainer(not_key_value_container_t, const T& value);

  template<typename InputIterator, typename Func>
  void writeIterator(InputIterator first, InputIterator last, Func f);


  // chrono types
  template<typename Rep, typename Period>
  void write(const std::chrono::duration<Rep, Period>& duration);

  template<typename Duration>
  void write(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point);

  template<typename Clock, typename Duration>
  void write(const std::chrono::time_point<Clock, Duration>& time_point);

  void write(const std::time_t* tt);


  // tuple-like types
  template<typename ...Ts>
  inline void write(const std::tuple<Ts...>& value) { writeTuple(value, std::index_sequence_for<Ts...>{});}

  template<typename ...Ts>
  inline void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<>) { stream_ << "{}"; }

  template<typename ...Ts>
  inline void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0>) { stream_ << "{ 0 }"; }

  template<typename ...Ts, std::size_t ...I>
  void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0, I...>);

  template<typename T1, typename T2>
  void write(const std::pair<T1, T2>& value, const std::string& sep = ", " /* use " : " for key-value */);


  // integer sequence
  template<typename T, T ...v>
  inline void write(const std::integer_sequence<T, v...>&) { write(std::make_tuple(v...)); }
};

template<typename Stream>
template<typename T, VCCC_ENABLE_IF_IMPL(is_container_v<T>)>
void StreamWrapper<Stream>::write(const T& value) {
  if(use_key_value_separator)
    writeContainer(is_key_value_container_t<T>{}, value);
  else
    writeContainer(not_key_value_container_t{}, value);
}

template<typename Stream>
template<typename T>
void StreamWrapper<Stream>::writeContainer(StreamWrapper::key_value_container_t, const T& value) {
  writeIterator(value.begin(), value.end(), [this](const auto& p){write(p, " : ");});
}

template<typename Stream>
template<typename T>
void StreamWrapper<Stream>::writeContainer(StreamWrapper::not_key_value_container_t, const T& value) {
  writeIterator(value.begin(), value.end(), [this](const auto& elem){*this << elem;});
}

template<typename Stream>
template<typename InputIterator, typename Func>
void StreamWrapper<Stream>::writeIterator(InputIterator first, InputIterator last, Func f) {
  stream_ << '{';
  if (first != last) {
    stream_ << ' ';
    f(*first);
    ++first;
    for(; first != last; ++first) {
      stream_ << ", ";
      f(*first);
    }
    stream_ << ' ';
  }
  stream_ << '}';
}

template<typename Stream>
template<typename Rep, typename Period>
void StreamWrapper<Stream>::write(const std::chrono::duration<Rep, Period>& duration) {
  if(duration < std::chrono::minutes(1)) {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    if(duration < std::chrono::milliseconds(1)) {
      if(duration < std::chrono::nanoseconds(1'000))
        stream_ << ns.count() << "ns";
      else
        stream_ << ns.count() / 1'000.L << "us";
    }
    else { // duration >= std::chrono::milliseconds(1)
      if(duration < std::chrono::milliseconds(1'000))
        stream_ << ns.count() / 1'000'000.L << "ms";
      else
        stream_ << ns.count() / 1'000'000'000.L << "s";
    }
  }
  else {
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration);
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);

    if(duration < std::chrono::minutes(60)) {
      stream_ << mins.count() << "m "
              << (secs-mins).count() << "s";
    }
    else {
      auto hrs = std::chrono::duration_cast<std::chrono::hours>(duration);
      stream_ << hrs.count() << "h "
              << (mins - hrs).count() << "m "
              << (secs - mins).count() << "s";
    }
  }
}

template<typename Stream>
template<typename Duration>
void StreamWrapper<Stream>::write(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point) {
  static std::mutex localtime_m;

  auto tt = std::chrono::system_clock::to_time_t(time_point);

  std::unique_lock<std::mutex> lck(localtime_m);
  const std::tm* tm_obj = std::localtime(&tt);

  if(tm_obj == nullptr) // failed parsing
    return std::to_string(tt);

  stream_ << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
  lck.unlock();

  // calculate milliseconds
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())
      - std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
  stream_ << std::setfill('0') << std::setw(3) << ms.count();
}

template<typename Stream>
template<typename Clock, typename Duration>
void StreamWrapper<Stream>::write(const std::chrono::time_point<Clock, Duration>& time_point) {
  using system_clock = std::chrono::system_clock;

  // TODO: use custom steady_clock output instead of converting to system_clock
  static const auto system_clock_begin = system_clock::now();
  static const auto custom_clock_begin = Clock::now();
  static const auto gap =
      system_clock_begin.time_since_epoch() - custom_clock_begin.time_since_epoch();

  auto d = std::chrono::duration_cast<std::chrono::system_clock::duration>(time_point.time_since_epoch() + gap);
  write(std::chrono::time_point<std::chrono::system_clock>() + d);
}

template<typename Stream>
void StreamWrapper<Stream>::write(const std::time_t *tt) {
  static std::mutex localtime_m;
  std::lock_guard<std::mutex> lck(localtime_m);

#pragma warning( disable : 4996)
  const std::tm* tm_obj = std::localtime(tt);
  if(tm_obj == nullptr) // failed parsing
    return

  stream_ << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
}

template<typename Stream>
template<typename ...Ts, std::size_t ...I>
void StreamWrapper<Stream>::writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0, I...>) {
  stream_ << "{ ";
  *this << std::get<0>(value);
  int dummy[sizeof...(I)] = {
      (*this << ", " << std::get<I>(value), 0)...
  };
  stream_ << " }";
}

template<typename Stream>
template<typename T1, typename T2>
void StreamWrapper<Stream>::write(const std::pair<T1, T2>& value, const std::string& sep) {
  stream_ << "{ ";
  *this << value.first;
  stream_ << sep;
  *this << value.second;
  stream_ << " }";
}

} // namespace vccc

# endif //VCCC_LOG_STREAM_WRAPPER_HPP_
