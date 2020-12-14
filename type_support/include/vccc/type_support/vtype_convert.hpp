# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
# define VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
#
# include <algorithm>
# include "vccc/type_support/convert_to.hpp"
# include "vccc/type_traits.hpp"


namespace vccc{

/**
 *  changes the value_type
 */


/** opencv types */

template<typename NewType, template<typename, int...> class CVType, typename OldType, int ...CVParams,
        VCCC_REQUIRE((!std::is_same<NewType, OldType>::value))>
decltype(auto) vtype_convert(const CVType<OldType, CVParams...>& cv_type) {
  return convert_to<CVType<NewType, CVParams...>>(cv_type);
}

template<typename NewType, template<typename, int...> class CVType, int ...CVParams>
decltype(auto) vtype_convert(const CVType<NewType, CVParams...>& cv_type) {
  return cv_type;
}

template<typename NewType, template<typename, int...> class CVType, int ...CVParams>
decltype(auto) vtype_convert(CVType<NewType, CVParams...>&& cv_type) {
  return cv_type;
}


/** container types */

template<typename NewType, template<typename...> class Container, typename OldType, typename ...Params,
        VCCC_REQUIRE((is_container_v<Container<OldType, Params...>>,
                      !std::is_same<NewType, OldType>::value))>
decltype(auto) vtype_convert(const Container<OldType, Params...>& container) {
  Container<NewType, std::allocator<NewType>> res(container.size());
  std::transform(std::begin(container), std::end(container), std::begin(res),
                 [](auto &val) { return static_cast<NewType>(val); });
  return res;
}

template<typename NewType, template<typename...> class Container, typename ...Params,
        VCCC_REQUIRE((is_container_v<Container<NewType, Params...>>))>
decltype(auto) vtype_convert(const Container<NewType, Params...>& container) {
  return container;
}

template<typename NewType, template<typename...> class Container, typename ...Params,
        VCCC_REQUIRE((is_container_v<Container<NewType, Params...>>))>
decltype(auto) vtype_convert(Container<NewType, Params...>&& container) {
  return container;
}


/** container types with custom unary operation */

template<typename NewType, typename Func, template<typename...> class Container, typename OldType, typename ...Params,
        VCCC_REQUIRE((is_container_v<Container<OldType, Params...>>,
                      !std::is_same<NewType, OldType>::value))>
decltype(auto) vtype_convert(const Container<OldType, Params...>& container, Func func) {
  Container<NewType, std::allocator<NewType>> res(container.size());
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}


template<typename NewType, template<typename...> class Container, typename ...Params, typename UnaryOperation,
        VCCC_REQUIRE((is_container_v<Container<NewType, Params...>>))>
decltype(auto) vtype_convert(const Container<NewType, Params...>& container, UnaryOperation func) {
  return container;
}


/** std::array specialization */

template<typename NewType, typename OldType, std::size_t n,
        VCCC_REQUIRE((!std::is_same<NewType, OldType>::value))>
constexpr decltype(auto)
vtype_convert(const std::array<OldType, n>& container) {
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res),
                 [](auto& val) { return static_cast<NewType>(val); });
  return res;
}

template<typename NewType, std::size_t n>
constexpr decltype(auto)
vtype_convert(const std::array<NewType, n>& container) {
  return container;
}

template<typename NewType, std::size_t n>
constexpr decltype(auto)
vtype_convert(std::array<NewType, n>&& container) {
  return container;
}


/** std::array with custom unary operation specialization */

template<typename NewType, typename OldType, std::size_t n, typename UnaryOperation,
        VCCC_REQUIRE((!std::is_same<NewType, OldType>::value))>
constexpr decltype(auto)
vtype_convert(const std::array<OldType, n>& container, UnaryOperation func) {
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}

template<typename NewType, std::size_t n, typename UnaryOperation>
constexpr decltype(auto)
vtype_convert(const std::array<NewType, n>& container, UnaryOperation func) {
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}

template<typename NewType, std::size_t n, typename UnaryOperation>
constexpr decltype(auto)
vtype_convert(std::array<NewType, n>&& container, UnaryOperation func) {
  std::for_each(std::begin(container), std::end(container), func);
  return container;
}






}

# endif //VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
