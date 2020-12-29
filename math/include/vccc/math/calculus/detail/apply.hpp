# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_CALCULUS_DETAIL_APPLY_HPP
# define VCCC_MATH_CALCULUS_DETAIL_APPLY_HPP
#
# include <cstdlib>
# include <utility>
# include <tuple>

namespace vccc{ namespace detail{ namespace math{

template<typename Func, typename Tuple, std::size_t ...I, typename ...Args>
decltype(auto)
applyTupleAndVariadicsImpl(Func&& f, Tuple&& tuple, std::index_sequence<I...>, Args&&... args)
{
  return std::forward<Func>(f)(std::get<I>(std::forward<Tuple>(tuple))..., std::forward<Args>(args)...);
}

template<typename Func, typename Tuple, typename ...Args>
decltype(auto)
applyTupleAndVariadics(Func&& f, Tuple&& tup, Args&&... args)
{
  return applyTupleAndVariadicsImpl(std::forward<Func>(f),
                                    std::forward<Tuple>(tup),
                                    std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{},
                                    std::forward<Args>(args)...);
}

}}}

# endif //VCCC_MATH_CALCULUS_DETAIL_APPLY_HPP
