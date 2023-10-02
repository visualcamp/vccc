# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef VCCC_OPTIONAL_HASH_H_
# define VCCC_OPTIONAL_HASH_H_
#
# include <cstddef>
#
# include <functional>
# include <type_traits>
#
# include "vccc/optional/internal/hash.h"
# include "vccc/optional/optional.h"

namespace std {

template<typename T>
struct hash<vccc::optional<T>> : vccc::internal::optional::hash_constructible<std::remove_const_t<T>> {
  using argument_type = vccc::optional<T>;
  using result_type = std::size_t;

  result_type operator()(const argument_type& key) const {
    return key.has_value() ? ::std::hash<std::remove_const_t<T>>()(*key) : 0;
  }
};

} // namespace std

# endif //VCCC_OPTIONAL_HASH_H_
