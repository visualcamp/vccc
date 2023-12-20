# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_RANGE_RANGE_HPP
# define VCCC_RANGE_RANGE_HPP
#
# include <algorithm>
# include <iterator>
# include <numeric>
# include <utility>
# include <vector>

namespace vccc {

//! @addtogroup range
//! @{

// TODO(Tony): Replace with ranges::iota_view
template<typename T, typename Container = std::vector<T>>
class range {
 public:
  using value_type = T;
  using container_type = Container;
  using iterator = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;
  using reverse_iterator = typename container_type::reverse_iterator;
  using const_reverse_iterator = typename container_type::const_reverse_iterator;

  explicit range(value_type b)
    : container(b)
    { std::iota(begin(), end(), 0); }
  explicit range(value_type a, value_type b)
    : container((std::max)(b - a, 0))
    { std::iota(begin(), end(), a); }
  explicit range(value_type a, value_type b, value_type incr)
    : container((b-a) * incr > 0 ? (std::abs(b-a) - 1) / std::abs(incr) + 1 : 0)
    { fill(begin(), end(), a, incr); }

  inline iterator begin() { return container.begin(); }
  inline const_iterator begin() const { return container.begin(); }
  inline const_iterator cbegin() const { return container.cbegin(); }
  inline reverse_iterator rbegin() { return container.rbegin(); }
  inline const_reverse_iterator rbegin() const { return container.rbegin(); }
  inline const_reverse_iterator crbegin() const { return container.crbegin(); }

  inline iterator end() { return container.end(); }
  inline const_iterator end() const { return container.end(); }
  inline const_iterator cend() const { return container.cend(); }
  inline reverse_iterator rend() { return container.rend(); }
  inline const_reverse_iterator rend() const { return container.rend(); }
  inline const_reverse_iterator crend() const { return container.crend(); }

  inline operator Container() & {
    return container;
  }

  inline operator Container() && {
    return std::move(container);
  }

 protected:

  void fill(iterator first, iterator last, value_type a, value_type incr) {
    for(; first != last; ++first, a+=incr)
      *first = a;
  }

  Container container;
};

//! @}

} // namespace vccc

# endif // VCCC_RANGE_RANGE_HPP
