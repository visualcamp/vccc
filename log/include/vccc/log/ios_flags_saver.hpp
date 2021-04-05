# /*
#  * Created by YongGyu Lee on 2021/03/20.
#  */
#
# ifndef VCCC_LOG_IOS_FLAGS_SAVER_HPP_
# define VCCC_LOG_IOS_FLAGS_SAVER_HPP_
#
# include <ios>


namespace vccc {

//! @addtogroup log
//! @{

/**

@brief RAII wrapper that restores stream's flags on destroyed

@tparam Stream stream type

@code{.cpp}
{
  IOSFlagsSaver<std::ostream> saver(std::cout); // flags saved
  std::cout << std::setw(10) << 1;
} // flags restored
std::cout << 2;
@endcode

 */

template<typename Stream>
class IOSFlagsSaver {
 public:
  using stream_type = Stream;

  explicit IOSFlagsSaver(stream_type& stream) : stream(stream), flags(stream.flags()) {}
  ~IOSFlagsSaver() { stream.flags(flags); }

 private:
  std::ios_base::fmtflags flags;
  stream_type& stream;
};

//! @} log

}


#endif //VCCC_LOG_IOS_FLAGS_SAVER_HPP_
