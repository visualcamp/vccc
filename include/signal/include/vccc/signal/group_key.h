# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_SIGNAL_GROUP_KEY_H_
# define VCCC_SIGNAL_GROUP_KEY_H_
#
# include <utility>
#
# include "vccc/optional.hpp"

namespace vccc {

enum group_category { ungrouped_front, grouped, ungrouped_back };

template<typename Group>
struct group_key {
  using key_type = Group;

  group_key() = default;

  explicit group_key(group_category group_category_, key_type group_key)
    : group_token(std::make_pair(group_category_, group_key)) {}

  explicit group_key(group_category group_category_)
    : group_token(std::make_pair(group_category_, vccc::nullopt)) {}

//  explicit group_key(key_type group_key)
//    : group_token(std::make_pair(ungrouped_front, )) {}

  std::pair<group_category, vccc::optional<key_type>> group_token{ungrouped_front, vccc::nullopt};
};

template<typename Group>
bool operator==(const group_key<Group>& lhs, const group_key<Group>& rhs) {
  if (lhs.group_token.first != rhs.group_token.first)
    return false;
  if (lhs.group_token.first != grouped)
    return true;
  return lhs.group_token.second == rhs.group_token.second;
}

template<typename Group>
bool operator!=(const group_key<Group>& lhs, const group_key<Group>& rhs) {
  return !(lhs == rhs);
}

template<typename Group>
struct group_key_compare {
  using group_type = Group;
  using group_key_type = group_key<group_type>;
  inline bool operator()(const group_key_type& lhs, const group_key_type& rhs) const {
    if (lhs.group_token.first != rhs.group_token.first) {
      return lhs.group_token.first < rhs.group_token.first;
    }
    // group_category is same by now
    if (lhs.group_token.first == grouped) {
      return lhs.group_token.second < rhs.group_token.second;
    }
    return false;
  }
};

} // namespace vccc

# endif // VCCC_SIGNAL_GROUP_KEY_H_
