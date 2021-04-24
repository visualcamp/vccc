# /*
#  * Created by YongGyu Lee on 2021/04/24.
#  */
#
# ifndef VCCC_ACCESS_ACCESS_GET_H_
# define VCCC_ACCESS_ACCESS_GET_H_

namespace vccc {

template<typename Tag, typename Class>
struct AccessTag {
  using class_type = Class;
};

template<typename Tag, typename Type, Type Tag::class_type::* ptr_>
struct Accessor {
  using class_type = typename Tag::class_type;
  using member_type = Type;
  using pointer_type = member_type class_type::*;
  static constexpr pointer_type ptr = ptr_;

  friend Accessor get_accessor_type_impl(Tag);

  friend member_type& access(class_type& target, Tag);
//  friend const member_type& access(const class_type& target);

//  friend member_type&& access(Tag, class_type&& target) noexcept { return target.*ptr; }
//  friend const member_type&& access(Tag, const class_type&& target) noexcept { return target.*ptr; }
};

template<typename Tag>
struct AccessTagTraits {
  using tag_type = Tag;
  using accessor_type = decltype(get_accessor_type_impl(tag_type()));
  using class_type = typename tag_type::class_type;
  using member_type = typename tag_type::member_type;
  using pointer_type = typename accessor_type::pointer_type;
};

template<typename Tag>
struct matching_accessor {
  using type = decltype(get_accessor_type_impl(Tag()));
};

template<typename Tag>
using matching_accessor_t = typename matching_accessor<Tag>::type;

template<typename Tag>
typename matching_accessor_t<Tag>::member_type&
access(typename Tag::class_type& target) {
  return target.*matching_accessor_t<Tag>::ptr;
}

}

# endif //VCCC_ACCESS_ACCESS_GET_H_
