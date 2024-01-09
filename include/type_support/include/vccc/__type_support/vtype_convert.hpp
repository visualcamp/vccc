# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
# define VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
#
# include <algorithm>
# include "vccc/__type_support/convert_to.hpp"
# include "vccc/__type_support/cast.hpp"
# include "vccc/type_traits.hpp"

namespace vccc {

/**
@addtogroup type_support
@{
    @defgroup type_support_vtype_convert vccc::vtype_convert
    @brief returns similar class with different value_type
@}

@addtogroup type_support_vtype_convert
@{
*/



/**
 * @brief returns similar opencv type, but the value_type is different
 *
 * @tparam NewType
 * @tparam CVType
 * @tparam OldType
 * @tparam CVParams
 * @param cv_type
 * @return
 */
template<typename NewType, template<typename, int...> class CVType, typename OldType, int ...CVParams,
         std::enable_if_t<!std::is_same<NewType, OldType>::value, int> = 0>
inline decltype(auto) vtype_convert(const CVType<OldType, CVParams...>& cv_type)
{
  return convert_to<CVType<NewType, CVParams...>>(cv_type);
}


//! @cond ignored
template<typename NewType, template<typename, int...> class CVType, int ...CVParams>
inline decltype(auto) vtype_convert(const CVType<NewType, CVParams...>& cv_type)
{
  return cv_type;
}

template<typename NewType, template<typename, int...> class CVType, int ...CVParams>
inline decltype(auto) vtype_convert(CVType<NewType, CVParams...>&& cv_type)
{
  return cv_type;
}
//! @endcond


/**
 * @brief vtype_convert on container types with custom unary operation
 *
 * @tparam NewType
 * @tparam Func
 * @tparam Container
 * @tparam OldType
 * @tparam Params
 * @param container
 * @param func
 * @return
 */
template<typename NewType, typename Func, template<typename...> class Container, typename OldType, typename ...Params,
    std::enable_if_t<is_range<Container<OldType, Params...>>::value, int> = 0>
decltype(auto) vtype_convert(const Container<OldType, Params...>& container, Func func)
{
  Container<NewType, std::allocator<NewType>> res(container.size());
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}

template<typename NewType, template<typename...> class Container, typename ...Params, typename UnaryOperation,
    std::enable_if_t<is_range<Container<NewType, Params...>>::value, int> = 0>
inline decltype(auto) vtype_convert(const Container<NewType, Params...>& container, UnaryOperation func)
{
  return container;
}


/**
 * @brief returns similar container type, but the value_type is different
 *
 * @tparam NewType
 * @tparam Container
 * @tparam OldType
 * @tparam Params
 * @param container
 * @return
 */
template<typename NewType, template<typename...> class Container, typename OldType, typename ...Params,
    std::enable_if_t<is_range<Container<OldType, Params...>>::value, int> = 0>
decltype(auto) vtype_convert(const Container<OldType, Params...>& container)
{
  return vtype_convert<NewType>(container, [](auto &val) { return cast<NewType>(val); });
}


template<typename NewType, template<typename...> class Container, typename ...Params,
    std::enable_if_t<is_range<Container<NewType, Params...>>::value, int> = 0>
inline decltype(auto) vtype_convert(const Container<NewType, Params...>& container)
{
  return container;
}

template<typename NewType, template<typename...> class Container, typename ...Params,
    std::enable_if_t<is_range<Container<NewType, Params...>>::value, int> = 0>
inline decltype(auto) vtype_convert(Container<NewType, Params...>&& container)
{
  return std::move(container);
}


/**
 * std::array with custom unary operation specialization
 *
 * @tparam NewType
 * @tparam OldType
 * @tparam n
 * @tparam UnaryOperation
 * @param container
 * @param func
 * @return
 */
template<typename NewType, typename OldType, std::size_t n, typename UnaryOperation,
    std::enable_if_t<!std::is_same<NewType, OldType>::value, int> = 0>
constexpr decltype(auto)
vtype_convert(const std::array<OldType, n>& container, UnaryOperation func)
{
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}

template<typename NewType, std::size_t n, typename UnaryOperation>
constexpr decltype(auto)
vtype_convert(const std::array<NewType, n>& container, UnaryOperation func)
{
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res), func);
  return res;
}

template<typename NewType, std::size_t n, typename UnaryOperation>
constexpr decltype(auto)
vtype_convert(std::array<NewType, n>&& container, UnaryOperation func)
{
  std::for_each(std::begin(container), std::end(container), func);
  return container;
}


/** std::array specialization */

template<typename NewType, typename OldType, std::size_t n,
    std::enable_if_t<!std::is_same<NewType, OldType>::value, int> = 0>
constexpr decltype(auto)
vtype_convert(const std::array<OldType, n>& container)
{
  std::array<NewType, n> res;
  std::transform(std::begin(container), std::end(container), std::begin(res),
                 [](auto& val) { return cast<NewType>(val); });
  return res;
}

template<typename NewType, std::size_t n>
constexpr inline decltype(auto)
vtype_convert(const std::array<NewType, n>& container)
{
  return container;
}

template<typename NewType, std::size_t n>
constexpr inline decltype(auto)
vtype_convert(std::array<NewType, n>&& container)
{
  return std::move(container);
}

//! @} type_support_vtype_convert

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_VTYPE_CONVERT_HPP
