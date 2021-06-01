# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef VCCC_OPTIONAL_INTERNAL_DTOR_H_
# define VCCC_OPTIONAL_INTERNAL_DTOR_H_
#
# include <type_traits>
# include <utility>
#
# include "vccc/optional/inplace_t.h"

namespace vccc {
namespace internal {
namespace optional {

template<typename T, bool v = std::is_trivially_destructible<T>::value>
struct dtor {
  using value_type = T;

  constexpr dtor() noexcept
    : null{} {}

  template<typename ...Args>
  constexpr explicit dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  void reset() {
    if (valid) {
      val.value_type::~value_type();
      valid = false;
    }
  }

  constexpr inline const value_type* pointer() const { return &val; }
  constexpr inline       value_type* pointer()       { return &val; }

  constexpr inline const value_type& ref() const&  { return val;             }
  constexpr inline       value_type& ref()      &  { return val;             }
  constexpr inline const value_type& ref() const&& { return std::move(val);  }
  constexpr inline       value_type& ref()      && { return std::move(val);  }

  template<typename ...U>
  void construct(U&&... args) {
    ::new((void*)&val) value_type(std::forward<U>(args)...);
    valid = true;
  }

  template<typename U>
  void construct_if(U&& arg) {
    if (valid)
      construct(std::forward<U>(arg));
  }

  ~dtor() = default;

  union {
    char null;
    value_type val;
  };
  bool valid = false;
};

template<typename T>
struct dtor<T, false> {
  using value_type = T;

  constexpr explicit dtor() noexcept
    : null{} {}

  template<typename ...Args>
  constexpr dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  inline void reset() {
    if (valid) {
      val.T::~T();
      valid = false;
    }
  }

  constexpr inline const value_type* pointer() const { return &val; }
  constexpr inline       value_type* pointer()       { return &val; }

  constexpr inline const value_type& ref() const&  { return val;             }
  constexpr inline       value_type& ref()      &  { return val;             }
  constexpr inline const value_type& ref() const&& { return std::move(val);  }
  constexpr inline       value_type& ref()      && { return std::move(val);  }

  template<typename U>
  void construct(U&& arg) {
    ::new((void*)&val) value_type(std::forward<U>(arg));
    valid = true;
  }

  ~dtor() {
    if (valid)
      val.T::~T();
  };

  union {
    char null;
    T val;
  };
  bool valid = false;
};

} // namespace optional
} // namespace internal
} // namespace vccc

# endif // VCCC_OPTIONAL_INTERNAL_DTOR_H_
