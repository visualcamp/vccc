# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
# define VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
#
# include <iterator>

namespace vccc{

namespace internal {

class back_emplace_iterator_base;

# if __cplusplus >= 201703L
struct back_emplace_iterator_base {
  using iterator_category = std::output_iterator_tag;

  using value_type      = void;
# if __cplusplus >= 202002L
  using difference_type = std::ptrdiff_t;
# else
  using difference_type = void;
# endif
  using pointer         = void;
  using reference       = void;
};
# else
struct back_emplace_iterator_base
    : public std::iterator<std::output_iterator_tag,
                           void,
                           void,
                           void,
                           void> {};
# endif

} // namespace internal

//! @addtogroup type_support
//! @{

template <class Container>
class back_emplace_iterator
    : public internal::back_emplace_iterator_base
{
 protected:
  Container* container;
 public:
  using container_type = Container;

  explicit back_emplace_iterator(Container& x) : container(std::addressof(x)) {}
  back_emplace_iterator& operator=(const typename Container::value_type& value)
  {container->emplace_back(value); return *this;}
  back_emplace_iterator& operator=(typename Container::value_type&& value)
  {container->emplace_back(std::move(value)); return *this;}
  back_emplace_iterator& operator*()     {return *this;}
  back_emplace_iterator& operator++()    {return *this;}
  back_emplace_iterator  operator++(int) {return *this;}
};

/**
@brief make back_emplace_iterator
@param container    container
@return back_emplace_iterator
 */
template<typename Container>
back_emplace_iterator<Container>
back_emplacer(Container& container)
{
  return back_emplace_iterator<Container>(container);
}

//! @}

}

# endif //VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
