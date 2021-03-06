# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_REQUIRE_HPP
# define VCCC_TYPE_TRAITS_REQUIRE_HPP


# define VCCC_REQUIRE(Cond) std::enable_if_t<Cond, int> = 0
# define VCCC_REQUIRE_FORWARD(Cond) VCCC_REQUIRE(Cond)
# define VCCC_REQUIRE_IMPL(Cond) std::enable_if_t<Cond, int>


# endif //VCCC_TYPE_TRAITS_REQUIRE_HPP
