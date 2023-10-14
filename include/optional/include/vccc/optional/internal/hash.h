# /*
#  * Created by YongGyu Lee on 2021/05/25.
#  */
#
# ifndef VCCC_OPTIONAL_INTERNAL_HASH_H_
# define VCCC_OPTIONAL_INTERNAL_HASH_H_
#
# include <functional>
# include <type_traits>

namespace vccc {
namespace internal {
namespace optional {

struct not_constructible {
  not_constructible() = delete;
};

struct constructible {};

template<typename T>
using hash_constructible =
std::conditional_t<
  std::is_default_constructible<std::hash<T>>::value,
  constructible,
  not_constructible>;

} // namespace optional
} // namespace internal
} // namespace vccc

# endif // VCCC_OPTIONAL_INTERNAL_HASH_H_
