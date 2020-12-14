# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
# define VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
#
# include <iterator>

namespace vccc{

template <class Container>
class back_emplace_iterator
    : public std::iterator<std::output_iterator_tag,
                           void,
                           void,
                           void,
                           void>
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

template<typename Container>
back_emplace_iterator<Container>
back_emplacer(Container& container){
  return back_emplace_iterator<Container>(container);
}

}

# endif //VCCC_TYPE_SUPPORT_BACK_EMPLACE_HPP
