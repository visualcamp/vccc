# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_HIGH_FUNCTIONAL_DETAIL_INVOKE_HPP
# define VCCC_HIGH_FUNCTIONAL_DETAIL_INVOKE_HPP
#
# include <utility>
# include <type_traits>

namespace vccc{ namespace detail {


template <class>
constexpr bool is_reference_wrapper_v = false;
template <class U>
constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;

struct invoke_category_base_of{};
struct invoke_category_reference_wrapper{};
struct invoke_category_normal{};

template<class T, class T1>
struct get_invoke_category {
  using type
  = std::conditional_t<
      std::is_base_of<T, std::decay_t<T1>>::value,
        invoke_category_base_of,
        std::conditional_t<
          is_reference_wrapper_v<std::decay_t<T1>>,
            invoke_category_reference_wrapper,
            invoke_category_normal>>;
};

template<class T, class T1>
using get_invoke_category_t = typename get_invoke_category<T, T1>::type;

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP_MF(invoke_category_base_of, Type T::* f, T1&& t1, Args&&... args) {
  return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP_MF(invoke_category_reference_wrapper, Type T::* f, T1&& t1, Args&&... args) {
  return (t1.get().*f)(std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP_MF(invoke_category_normal, Type T::* f, T1&& t1, Args&&... args) {
  return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP(invoke_category_base_of, Type T::* f, T1&& t1, Args&&... args) {
  return std::forward<T1>(t1).*f;
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP(invoke_category_reference_wrapper, Type T::* f, T1&& t1, Args&&... args) {
  return t1.get().*f;
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKEP(invoke_category_normal, Type T::* f, T1&& t1, Args&&... args) {
  return (*std::forward<T1>(t1)).*f;
}


template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKE_TAG1(std::true_type, Type T::* f, T1&& t1, Args&&... args)
{
  return INVOKEP_MF(get_invoke_category_t<T, T1>{}, f, std::forward<T1>(t1), std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKE_TAG1(std::false_type, Type T::* f, T1&& t1, Args&&... args)
{
  return INVOKEP(get_invoke_category_t<T, T1>{}, f, std::forward<T1>(t1), std::forward<Args>(args)...);
}

template <class T, class Type, class T1, class... Args>
constexpr inline
decltype(auto)
INVOKE(Type T::* f, T1&& t1, Args&&... args)
{
  return INVOKE_TAG1(std::is_member_function_pointer<decltype(f)>{}, f, std::forward<T1>(t1), std::forward<Args>(args)...);
}

template <class F, class... Args>
constexpr inline
decltype(auto)
INVOKE(F&& f, Args&&... args)
{
  return std::forward<F>(f)(std::forward<Args>(args)...);
}


}}

#endif //VCCC_HIGH_FUNCTIONAL_DETAIL_INVOKE_HPP
