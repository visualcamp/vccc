# /*
#  * Created by YongGyu Lee on 2021/03/20.
#  */
#
# ifndef VCCC_LOG_STREAM_WRAPPER_HPP_
# define VCCC_LOG_STREAM_WRAPPER_HPP_
#
# include <chrono>
# include <functional>
# include <ostream>
# include <iomanip>
# include <mutex>
# include <ratio>
# include <sstream>
# include <string>
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "vccc/optional.hpp"
# include "vccc/type_traits.hpp"
# include "vccc/log/ios_flags_saver.hpp"
#
# if __cplusplus >= 201703
#   include <filesystem>
#   include <optional>
#   include "boost/pfr.hpp"
# endif


namespace vccc {

//! @addtogroup log
//! @{

/**
@brief Manipulator for vccc::StreamWrapper
 */
class StreamManipulator {
 public:
  StreamManipulator() = default;
  ~StreamManipulator() = default;
};

/**
 *
 * @tparam CharT
 * @tparam String
 * @tparam Stream
 */
template<typename String, typename Stream>
class StreamWrapperBase {
 public:
  using stream_type = Stream;
  using string_type = String;
  using traits_type = typename stream_type::traits_type;

  /** @brief Get global separator
   *
   * Default value is an empty string
   * @return Global separator
   */
  static string_type& global_separator() {
    static string_type s;
    return s;
  }

  /** @brief Get current separator
   *
   * Default-initialized with StreamWrapperBase::global_separator()
   * @return Current separator
   */
  const string_type& separator() const { return separator_; }

  /** @brief Set current separator
   *
   * Set current separator with new value and return the previous one
   * @return Previous separator
   */
  string_type separator(string_type new_separator) {
    auto prev = std::move(separator_);
    separator_ = std::move(new_separator);
    return prev;
  }

 protected:
  struct {
    string_type separator_ = global_separator();
    bool first_ = true;
  };
};

/**
@brief stream wrapper that supports extended operator overloading

Support writing of
 - container types*
 - tuple-like types*
 - container whose element is pair is treated as key-pair container
 - std::chrono types
 - integer sequences*
 - (of course) custom operator overloaded types (std::ostream& operator << (std::ostream&, T))
 - aggregate type*

Note: Types marked with (*) are default-separated with a single space

@tparam CharT
@tparam String
@tparam Stream
 */
template<typename CharT,
         typename String = std::basic_string<CharT>,
         typename Stream = std::basic_stringstream<CharT>>
class BasicStreamWrapper : public StreamWrapperBase<String, Stream> {
 public:
  using base = StreamWrapperBase<String, Stream>;
  using char_type = CharT;
  using stream_type = typename base::stream_type;
  using string_type = typename base::string_type;
  using traits_type = typename base::traits_type;

  BasicStreamWrapper() = default;
  explicit BasicStreamWrapper(string_type str) : stream_(std::move(str)) {}
  explicit BasicStreamWrapper(stream_type&& stream) : stream_(std::move(stream)) {}

  // for std::endl
  BasicStreamWrapper& operator << (std::ostream& (*pf)(std::ostream&)) {
    pf(stream_);
    return *this;
  }

  // I/O manipulators
  BasicStreamWrapper& operator << (std::ios_base& (&io_manip)(std::ios_base&)) {
    stream_ << io_manip;
    return *this;
  }

  template<typename T, typename
    std::enable_if_t<
      std::is_base_of<StreamManipulator, remove_cvref_t<T>>::value,
    int> = 0>
  BasicStreamWrapper& operator << (const T& manipulator) {
    manipulator(*this);
    return *this;
  }

  template<typename T, typename
    std::enable_if_t<
      !std::is_base_of<StreamManipulator, remove_cvref_t<T>>::value,
    int> = 0>
  BasicStreamWrapper& operator << (const T& value) {
    if (base::first_) {
      base::first_ = false;
    } else {
      stream_ << base::separator();
    }
    try_write(typename is_printable<T>::type{}, value);
    return *this;
  }

  /** @brief Return internal stream
   *
   * @return Internal stream object
   */
  stream_type& stream() { return stream_; }
  const stream_type& stream() const { return stream_; }

  /** @brief Equals to stream().str()
   *
   * @return stream().str()
   */
  string_type str() const { return stream().str(); }

   /** @brief Equals to stream().str(string_type)
    *
    * @param s New string
    */
  void str(string_type s) { stream().str(std::move(s)); }

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
  constexpr void try_write(not_default_printable_t, const T& value) { write(value); }

#if __cplusplus >= 201703
  // any aggregate type
  template<typename T, std::enable_if_t<!is_range<T>::value && std::is_aggregate_v<T>, int> = 0>
  void write(const T& aggr) {
    writeAggregate(aggr, std::make_index_sequence<boost::pfr::tuple_size_v<T>>{});
  }

  template<typename T>
  void writeAggregate(const T& aggr, std::index_sequence<>) {
    stream_ << "{}";
  }

  template<typename T>
  void writeAggregate(const T& aggr, std::index_sequence<0>) {
    stream_ << "{ ";
    (*this) << boost::pfr::get<0>(aggr);
    stream_ << " }";
  }

  template<typename T, std::size_t ...I>
  void writeAggregate(const T& aggr, std::index_sequence<0, I...>) {
    stream_ << "{ ";
    (*this) << boost::pfr::get<0>(aggr);
    (((*this) << ", " << boost::pfr::get<I>(aggr) ), ...);
    stream_ << " }";
  }
#endif

  // container types
  template<typename T, VCCC_ENABLE_IF_FORWARD(is_range_v<T>)>
  inline void write(const T& value) {
    use_key_value_separator?
      writeContainer(is_key_value_container_t<T>{}, value):
      writeContainer(not_key_value_container_t{}, value);
  }

  template<typename T>
  inline void writeContainer(key_value_container_t, const T& value)
  { writeIterator(value.begin(), value.end(), [this](const auto& p){write(p, ": ");}); }

  template<typename T>
  inline void writeContainer(not_key_value_container_t, const T& value)
  { writeIterator(value.begin(), value.end(), [this](const auto& elem){*this << elem;}); }

  template<typename InputIterator, typename Func>
  void writeIterator(InputIterator first, InputIterator last, Func f);


  // chrono types
  template<typename Rep, typename Period>
  void write(const std::chrono::duration<Rep, Period>& duration);

  template<typename Duration>
  void write(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point);

  template<typename Clock, typename Duration>
  inline void write(const std::chrono::time_point<Clock, Duration>& time_point) {
    stream_ << "+";
    write(time_point.time_since_epoch());
  }

  void write(const std::time_t* tt);


  // tuple-like types
  template<typename ...Ts>
  inline void write(const std::tuple<Ts...>& value) {
    writeTuple(value, std::index_sequence_for<Ts...>{});
  }

  template<typename ...Ts>
  inline void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<>) {
    stream_ << "{}";
  }

  template<typename ...Ts>
  inline void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0>) {
    stream_ << "{ " << std::get<0>(value) << " }";
  }

  template<typename ...Ts, std::size_t ...I>
  void writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0, I...>);

  template<typename T1, typename T2>
  void write(const std::pair<T1, T2>& value, const std::string& sep = ", " /* use " : " for key-value */);


  // integer sequence
  template<typename T, T ...v>
  inline void write(const std::integer_sequence<T, v...>&) {
    write(std::make_tuple(v...));
  }

  template<typename T>
  void write(const vccc::optional<T>& op) {
    if (op)
      *this << *op;
    else
      stream_ << "nullopt";
  }

  // directory_entry(Apple Clang defects)
# if __cplusplus >= 201703L
  void write(const std::filesystem::directory_entry& d) {
    stream_ << d.path();
  }

  template<typename T>
  void write(const std::optional<T>& op) {
    if (op)
      *this << *op;
    else
      stream_ << "nullopt";
  }
# endif
};

template<typename CharT, typename String, typename Stream>
template<typename InputIterator, typename Func>
void BasicStreamWrapper<CharT, String, Stream>::writeIterator(InputIterator first, InputIterator last, Func f) {
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

template<typename CharT, typename String, typename Stream>
template<typename Rep, typename Period>
void BasicStreamWrapper<CharT, String, Stream>::write(const std::chrono::duration<Rep, Period>& duration_) {
  IOSFlagsSaver<stream_type> saver(stream_);

  auto duration =
      duration_ >= std::chrono::duration<Rep, Period>::zero() ?
      duration_ : -duration_;

  if(duration < std::chrono::minutes(1)) {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    stream_ << std::setprecision(5);

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
    using days    = std::chrono::duration<long, std::ratio<   86400>>;
    using months  = std::chrono::duration<long, std::ratio< 2629746>>;
    using years   = std::chrono::duration<long, std::ratio<31556952>>;

    auto hh = std::chrono::duration_cast<std::chrono::hours       >(duration % days                (1)).count();
    auto mm = std::chrono::duration_cast<std::chrono::minutes     >(duration % std::chrono::hours  (1)).count();
    auto ss = std::chrono::duration_cast<std::chrono::seconds     >(duration % std::chrono::minutes(1)).count();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration % std::chrono::seconds(1)).count();

    if(duration >= days(1)) {
      auto DD = std::chrono::duration_cast<days  >(duration % months(1)).count();
      auto MM = std::chrono::duration_cast<months>(duration % years(1)).count();
      auto YY = std::chrono::duration_cast<years >(duration).count();

      if(YY) stream_ << YY << "Y ";
      if(MM) stream_ << MM << "M ";
      if(DD) stream_ << DD << "D ";
      stream_ << std::setw(2) << std::setfill(' ');
    }

    if(hh || mm || ss || ms) {
      if (duration >= std::chrono::hours(1))
        stream_ << hh << "h "
                << std::setw(2) << std::setfill(' ') << mm << "m ";
      else if (duration >= std::chrono::minutes(1))
        stream_ << mm << "m ";

      stream_ << std::setw(2) << std::setfill(' ') << ss;
      if (ms) stream_ << '.' << std::setw(3) << std::setfill('0') << ms;
      stream_ << 's';
    }
  }
}

template<typename CharT, typename String, typename Stream>
template<typename Duration>
void BasicStreamWrapper<CharT, String, Stream>::write(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point) {
  static auto localtime_m = new std::mutex();

  auto tt = std::chrono::system_clock::to_time_t(time_point);

  std::unique_lock<std::mutex> lck(*localtime_m);
  const std::tm* tm_obj = std::localtime(&tt);

  if (tm_obj == nullptr) {// failed parsing
    std::to_string(tt);
    return;
  }

  stream_ << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
  lck.unlock();

  // calculate milliseconds
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      time_point.time_since_epoch() % std::chrono::seconds(1)).count();
  IOSFlagsSaver<stream_type> saver(stream_);
  stream_ << std::setfill('0') << std::setw(3) << ms;
}

template<typename CharT, typename String, typename Stream>
void BasicStreamWrapper<CharT, String, Stream>::write(const std::time_t *tt) {
  static std::mutex localtime_m;
  std::lock_guard<std::mutex> lck(localtime_m);

#pragma warning( disable : 4996)
  const std::tm* tm_obj = std::localtime(tt);
  if(tm_obj == nullptr) // failed parsing
    return

  stream_ << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
}

template<typename CharT, typename String, typename Stream>
template<typename ...Ts, std::size_t ...I>
void BasicStreamWrapper<CharT, String, Stream>::writeTuple(const std::tuple<Ts...>& value, std::index_sequence<0, I...>) {
  stream_ << "{ ";
  *this << std::get<0>(value);
  int dummy[sizeof...(I)] = {
      (*this << ", " << std::get<I>(value), 0)...
  };
  stream_ << " }";
}

template<typename CharT, typename String, typename Stream>
template<typename T1, typename T2>
void BasicStreamWrapper<CharT, String, Stream>::write(const std::pair<T1, T2>& value, const std::string& sep) {
  stream_ << "{ ";
  *this << value.first;
  stream_ << sep;
  *this << value.second;
  stream_ << " }";
}

using StreamWrapper = BasicStreamWrapper<char>;

//! @} log

} // namespace vccc

# endif //VCCC_LOG_STREAM_WRAPPER_HPP_
