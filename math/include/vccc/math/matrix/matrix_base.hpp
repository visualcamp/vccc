# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MAT_EXPRESSION_HPP
# define VCCC_MATH_MAT_EXPRESSION_HPP
#
# include <cstddef>
# include <type_traits>

namespace vccc{

template<typename Derived>
class MatrixBase {
 public:
  using derived_type = Derived;
  using derived_traits = internal::math::traits<derived_type>;
  using value_type = typename derived_traits::value_type;

  enum {
    rows = derived_traits::rows,
    cols = derived_traits::cols,
    size = rows * cols,
    shortdim = rows < cols ? rows : cols
  };

  static_assert((rows > 0 && cols > 0), "matrix size must be greater than 0");

  //! static polymorphic virtual-like member functions
  constexpr value_type operator() (std::size_t i) const {
    return derived()(i);
  }
  constexpr value_type operator() (std::size_t i, std::size_t j) const {
    return derived()(i, j);
  }
  constexpr value_type operator[] (std::size_t i) const {
    return derived()[i];
  }

  constexpr inline const derived_type& derived() const {
    return static_cast<const derived_type&>(*this);
  }
  constexpr inline derived_type& derived() {
    return static_cast<derived_type&>(*this);
  }
};


namespace internal { namespace math {

template<typename Derived>
std::true_type is_matrix_impl(const vccc::MatrixBase<Derived>&);
std::false_type is_matrix_impl(...);

}} // namespace internal::math

template<typename T>
struct is_matrix : decltype(::vccc::internal::math::is_matrix_impl(std::declval<T>())) {};

}


#endif //VCCC_MATH_MAT_EXPRESSION_HPP
