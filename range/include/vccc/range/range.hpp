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
# include <vector>

namespace vccc{

//! @addtogroup range
//! @{

template<typename T, typename Container = std::vector<T>>
class range {
 public:
  using value_type = T;
  using container_type = Container;
  using iterator = typename Container::iterator;

  explicit range(value_type b)
    : container(b)
    { std::iota(begin(), end(), 0); }
  explicit range(value_type a, value_type b)
    : container(std::max(b - a, 0))
    { std::iota(begin(), end(), a); }
  explicit range(value_type a, value_type b, value_type incr)
    : container((b-a) * incr > 0 ? (std::abs(b-a) - 1) / std::abs(incr) + 1 : 0)
    { fill(begin(), end(), a, incr); }

  decltype(auto) begin() { return container.begin(); }
  decltype(auto) begin() const { return container.begin(); }
  decltype(auto) cbegin() { return container.cbegin(); }
  decltype(auto) cbegin() const { return container.cbegin(); }
  decltype(auto) rbegin() { return container.rbegin(); }
  decltype(auto) rbegin() const { return container.rbegin(); }
  decltype(auto) crbegin() { return container.crbegin(); }
  decltype(auto) crbegin() const { return container.crbegin(); }

  decltype(auto) end() { return container.end(); }
  decltype(auto) end() const { return container.end(); }
  decltype(auto) cend() { return container.cend(); }
  decltype(auto) cend() const { return container.cend(); }
  decltype(auto) rend() { return container.rend(); }
  decltype(auto) rend() const { return container.rend(); }
  decltype(auto) crend() { return container.crend(); }
  decltype(auto) crend() const { return container.crend(); }

  operator Container() & {
    return container;
  }

  operator Container() && {
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

}

# endif //VCCC_RANGE_RANGE_HPP
