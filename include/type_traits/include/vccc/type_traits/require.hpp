# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_REQUIRE_HPP
# define VCCC_TYPE_TRAITS_REQUIRE_HPP


# define VCCC_ENABLE_IF(Cond) std::enable_if_t<Cond, int> = 0
# define VCCC_ENABLE_IF_FORWARD(Cond) VCCC_ENABLE_IF(Cond)
# define VCCC_ENABLE_IF_IMPL(Cond) std::enable_if_t<Cond, int>


# endif //VCCC_TYPE_TRAITS_REQUIRE_HPP
