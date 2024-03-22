# /*
#  * Created by YongGyu Lee on 2021/03/20.
#  */
#
# ifndef VCCC_LOG_STREAM_WRAPPER_HPP
# define VCCC_LOG_STREAM_WRAPPER_HPP
#
# include <chrono>
# include <functional>
# include <ostream>
# include <iomanip>
# include <memory>
# include <mutex>
# include <ratio>
# include <sstream>
# include <string>
# include <tuple>
# include <type_traits>
# include <utility>
#
# include "vccc/__core/nodiscard.hpp"
# include "vccc/__log/global_stream_wrapper_settings.hpp"
# include "vccc/__log/ios_flags_saver.hpp"
# include "vccc/__memory/addressof.hpp"
# include "vccc/optional.hpp"
# include "vccc/__ranges/range.hpp"
# include "vccc/__ranges/range_value_t.hpp"
# include "vccc/__ranges/subrange.hpp"
# include "vccc/string_view.hpp"
# include "vccc/type_traits.hpp"
# include "vccc/variant.hpp"
#
# if __cplusplus >= 201703
#   include <filesystem>
#   include <optional>
#   include <string_view>
#   include <variant>
#   include "boost/pfr.hpp"
# endif

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct has_typename_key_type : std::false_type {};

template<typename T>
struct has_typename_key_type<T, void_t<typename T::key_type>> : std::true_type {};

template<typename T, typename = void>
struct has_typename_mapped_type : std::false_type {};

template<typename T>
struct has_typename_mapped_type<T, void_t<typename T::mapped_type>> : std::true_type {};

template<typename T, bool = is_range<T>::value>
struct is_mapped_range : std::false_type {};

template<typename T>
struct is_mapped_range<T, true>
    : conjunction<
        has_typename_key_type<T>,
        has_typename_mapped_type<T>,
        is_specialization<ranges::range_value_t<T>, std::pair>
    > {};

#if __cplusplus >= 201703L
template<typename T, bool = std::is_aggregate<T>::value /* false */>
struct empty_aggregate : std::false_type {};

template<typename T>
struct empty_aggregate<T, true> : bool_constant<boost::pfr::tuple_size<T>::value == 0> {};
#else
#endif

} // namespace detail

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

  VCCC_NODISCARD bool quote_string() const noexcept {
    return quote_string_;
  }

  void quote_string(bool new_value) noexcept {
    quote_string_ = new_value;
  }

  VCCC_NODISCARD bool expand_aggregate() const noexcept {
    return expand_aggregate_;
  }

  void expand_aggregate(bool new_value) noexcept {
    expand_aggregate_ = new_value;
  }

  VCCC_NODISCARD bool expand_array() {
    return expand_array_;
  }

  void expand_array(bool new_value) {
    expand_array_ = new_value;
  }

 protected:
  struct {
    string_type separator_ = global_separator();
    bool first_ = true;
    bool quote_string_ = GlobalStreamWrapperSettings::quote_string();
    bool expand_aggregate_ = GlobalStreamWrapperSettings::expand_aggregate();
    bool expand_array_ = GlobalStreamWrapperSettings::expand_array();
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
  BasicStreamWrapper& operator<<(std::ostream& (*pf)(std::ostream&)) {
    pf(stream_);
    return *this;
  }

  // I/O manipulators
  BasicStreamWrapper& operator<<(std::ios_base& (&io_manip)(std::ios_base&)) {
    stream_ << io_manip;
    return *this;
  }

  // Custom I/O manipulators
  template<typename T, std::enable_if_t<
      std::is_base_of<StreamManipulator, remove_cvref_t<T>>
  ::value, int> = 0>
  BasicStreamWrapper& operator<<(const T& manipulator) {
    manipulator(*this);
    return *this;
  }

  // All printable types
  template<typename T, std::enable_if_t<
      !std::is_base_of<StreamManipulator, remove_cvref_t<T>>
  ::value, int> = 0>
  BasicStreamWrapper& operator<<(const T& value) {
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

  // Default-printable types
  template<typename T>
  void try_write(default_printable_t, const T& value) {
    stream_ << value;
  }

  // Raw string
  template<std::size_t N>
  void try_write(default_printable_t, const char(&str)[N]) {
    stream_ << str;
  }

  // Array types
  template<typename T, std::size_t N>
  void try_write(default_printable_t, const T(&arr)[N]) {
    if (!base::expand_array() && is_printable<T>::value) {
      stream_ << arr;
      return;
    }

    if (N == 0) {
      stream_ << "{}";
      return;
    }

    stream_ << "{ ";
    *this << *arr;
    for (std::size_t i = 1; i < N; ++i) {
      stream_ << ", ";
      *this << *(arr + i);
    }
    stream_ << " }";
  }

  // Strings
  void try_write(default_printable_t, const std::string& s) {
    if (base::quote_string_)
      stream_ << "\"" << s << "\"";
    else
      stream_ << s;
  }

  void try_write(default_printable_t, const string_view& sv) {
    if (base::quote_string_)
      stream_ << "\"" << sv << "\"";
    else
      stream_ << sv;
  }

#if __cplusplus >= 201703L
  void try_write(default_printable_t, const std::string_view& sv) {
    if (base::quote_string_)
      stream_ << "\"" << sv << "\"";
    else
      stream_ << sv;
  }
#endif

  template<typename T>
  constexpr void try_write(not_default_printable_t, const T& value) {
    write(value);
  }

#if __cplusplus >= 201703
  // any aggregate type
  template<typename T, std::enable_if_t<conjunction<
      negation<ranges::range<T>>,
      std::is_aggregate<T>,
      negation<detail::empty_aggregate<T>>
  >::value, int> = 0>
  void write(const T& aggr) {
    if (base::expand_aggregate_)
      writeAggregate(aggr, std::make_index_sequence<boost::pfr::tuple_size_v<T>>{});
    else
      writeAddress(aggr);
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

#if __cplusplus < 201703L
  template<typename T, std::enable_if_t<!ranges::range<T>::value, int> = 0>
#else
  template<typename T, std::enable_if_t<conjunction<
      negation<ranges::range<T>>,
      disjunction<
          negation<std::is_aggregate<T>>,
          detail::empty_aggregate<T>
      >
  >::value, int> = 0>
#endif
  void write(const T& value) {
    writeAddress(value);
  }

  template<typename T>
  void writeAddress(const T& value) {
    stream_ << '@' << vccc::addressof(value);
  }

  // ranges
  template<typename T, std::enable_if_t<ranges::range<T>::value, int> = 0>
  void write(const T& value) {
    writeRange(value);
  }

  template<typename T>
  void writeRange(const T& value) {
    auto first = value.begin();
    auto last = value.end();

    if (first == last) {
      stream_ << "{}";
      return;
    }

    stream_ << "{ ";
    writeRangeElement(*first, detail::is_mapped_range<T>::value);
    for (++first; first != last; ++first) {
      stream_ << ", ";
      writeRangeElement(*first, detail::is_mapped_range<T>::value);
    }
    stream_ << " }";
  }

  template<typename T>
  void writeRangeElement(const T& value, bool) {
    (*this) << value;
  }

  template<typename T1, typename T2>
  void writeRangeElement(const std::pair<T1, T2>& value, bool as_map) {
    if (as_map)
      (*this) << value.first << " => " << value.second;
    else
      (*this) << "{" << value.first << ", " << value.second << "}";
  }

  // chrono types
  template<typename Rep, typename Period>
  void write(const std::chrono::duration<Rep, Period>& duration);

  template<typename Duration>
  void write(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point);

  template<typename Clock, typename Duration>
  void write(const std::chrono::time_point<Clock, Duration>& time_point) {
    stream_ << "+";
    write(time_point.time_since_epoch());
  }

  void write(const std::time_t* tt);

  // Tuple-like types
  template<typename ...Ts>
  void write(const std::tuple<Ts...>& value) {
    writeTupleLike(value, std::index_sequence_for<Ts...>{});
  }

  template<typename T1, typename T2>
  void write(const std::pair<T1, T2>& value) {
    writeTupleLike(value, std::index_sequence_for<T1, T2>{});
  }

  template<typename T>
  void writeTupleLike(const T&, std::index_sequence<>) {
    stream_ << "{}";
  }

  template<typename T>
  void writeTupleLike(const T& value, std::index_sequence<0>) {
    stream_ << "{ ";
    (*this) << std::get<0>(value);
    stream_ << " }";
    (*this) << "{ " << std::get<0>(value) << " }";
  }

  template<typename T, std::size_t... I>
  void writeTupleLike(const T& value, std::index_sequence<0, I...>) {
    stream_ << "{ ";
    (*this) << std::get<0>(value);

    int dummy[sizeof...(I)] = {
        (((*this) << ", " << std::get<I>(value)), 0)...
    };

    stream_ << " }";
  }

  // integer sequence
  template<typename T, T ...v>
  void write(const std::integer_sequence<T, v...>&) {
    writeTupleLike(std::make_tuple(v...), std::make_index_sequence<sizeof...(v)>{});
  }

  // optional
  template<typename T>
  void write(const optional<T>& op) {
    if (op)
      *this << *op;
    else
      stream_ << "nullopt";
  }

  // variant
  template<typename... T>
  void write(const variant<T...>& v) {
    if (v.valueless_by_exception()) {
      stream_ << "valueless_by_exception";
      return;
    }
    v.visit([this](const auto& elem) {
      *this << elem;
    });
  }

  template<bool v>
  void write(const std::integral_constant<bool, v>& t) {
    stream_ << (v ? "true_type" : "false_type");
  }

  // unique_ptr<T>
  template<typename T>
  inline void write(const std::unique_ptr<T>& p) {
    stream_ << "std::unique_ptr -> ";
    writePointer(p.get());
  }

  // shared_ptr<T>
  template<typename T>
  inline void write(const std::shared_ptr<T>& p) {
    stream_ << "std::shared_ptr -> ";
    writePointer(p.get());
  }

  // weak_ptr<T>
  template<typename T>
  inline void write(const std::weak_ptr<T>& p) {
    stream_ << "std::weak_ptr -> ";
    writePointer(p.get());
  }

  template<typename T>
  void writePointer(const T* ptr) {
    stream_ << ptr;
  }

  // directory_entry(Apple Clang defects)
# if __cplusplus >= 201703L
  void write(const std::filesystem::directory_entry& d) {
    stream_ << d.path();
  }

  // std::optional
  template<typename T>
  void write(const std::optional<T>& op) {
    if (op)
      *this << *op;
    else
      stream_ << "nullopt";
  }

  // std::variant
  template<typename... T>
  void write(const std::variant<T...>& v) {
    if (v.valueless_by_exception()) {
      stream_ << "valueless_by_exception";
      return;
    }

    std::visit([this](const auto& elem) {
      *this << elem;
    }, v);
  }
# endif
};

//! @cond ignored

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

  std::time_t tt = std::chrono::system_clock::to_time_t(time_point);

  std::unique_lock<std::mutex> lck(*localtime_m);
  const std::tm* tm_obj = std::localtime(&tt);

  if (tm_obj == nullptr) {// failed parsing
    const auto unix_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
    stream_ << unix_timestamp;
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

//! @endcond ignored

using StreamWrapper = BasicStreamWrapper<char>;

//! @} log

} // namespace vccc

# endif // VCCC_LOG_STREAM_WRAPPER_HPP
