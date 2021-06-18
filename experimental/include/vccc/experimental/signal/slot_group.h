# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_EXPERIMENTAL_SIGNAL_SLOT_GROUP_H_
# define VCCC_EXPERIMENTAL_SIGNAL_SLOT_GROUP_H_
#
# include <algorithm>
# include <iterator>
# include <list>
# include <map>
# include <memory>
# include <tuple>
# include <utility>
# include <vector>
#
# include "vccc/experimental/signal/group_key.h"

namespace vccc {
namespace experimental {

enum slot_position {
  at_back, at_front,
};

template<typename Group, typename Slot>
struct grouped_slot_list {
  using group_type = Group;
  using group_key_type = group_key<group_type>;
  using group_key_compare_type = group_key_compare<group_type>;

  using slot_type = Slot;
  using slot_ptr_type = std::shared_ptr<slot_type>;
  using list = std::list<slot_ptr_type>;
  using list_iterator = typename list::iterator;
  using map = std::map<group_key_type, list_iterator, group_key_compare_type>;
  using map_iterator = typename map::iterator;
  using map_const_iterator = typename map::const_iterator;

  using weak_slot_ptr_type = std::weak_ptr<slot_type>;

  using iterator = list_iterator;
  using const_iterator = typename list::const_iterator;

  using insert_token = std::pair<map_iterator, list_iterator>;
  using weak_vector = std::vector<std::pair<weak_slot_ptr_type, insert_token>>;

  grouped_slot_list()
    : list_(), map_(group_key_compare_type {}) {}

  // insert grouped slot
  insert_token insert(group_type group, slot_ptr_type slot, slot_position pos = at_back) {
    group_key_type key(grouped, group);
    if (pos == at_back)
      return insert_back(key, std::move(slot));
    else
      return insert_front(key, std::move(slot));
  }

  // insert ungrouped slot
  insert_token insert(slot_ptr_type slot, slot_position pos = at_back) {
    group_key_type key;
    if (pos == at_back) {
      key.group_token.first = ungrouped_back;
    } else {
      key.group_token.first = ungrouped_front;
    }
    return insert_back(key, std::move(slot));
  }

  insert_token insert_back(group_key_type group, slot_ptr_type slot) {
    auto pos = map_.upper_bound(group); // find a upper bound of a group_key
    if (pos == map_.end()) {
      auto l_pos = list_.emplace(list_.end(), std::move(slot));
      return {map_.emplace_hint(pos, group, l_pos), l_pos};
    } else {
      auto l_pos = list_.emplace(pos->second, std::move(slot));
      if (pos->first == group) { // if group exists
        return {pos, l_pos};
      }
      return {map_.emplace_hint(pos, group, l_pos), l_pos};
    }
  }

  insert_token insert_front(group_key_type group, slot_ptr_type slot) {
    auto pos = map_.lower_bound(group); // find a lower bound of a group_key
    if (pos == map_.end()) {
      auto l_pos = list_.emplace(list_.end(), std::move(slot));
      return {map_.emplace_hint(pos, group, l_pos), l_pos};
    } else {
      if (pos->first == group) { // if group exists
        auto l_pos = list_.emplace(pos->second, std::move(slot));
        pos->second = l_pos;
        return {pos, l_pos};
      } else { // group not exists
        auto l_pos = list_.emplace(pos->second, std::move(slot));
        auto g_pos = map_.emplace_hint(pos, group, l_pos);
        return {g_pos, l_pos};
      }
    }
  }

  iterator begin() { return list_.begin(); }
  const_iterator begin() const { return list_.begin(); }
  iterator end() { return list_.end(); }
  const_iterator end() const { return list_.end(); }

  auto size() const { return list_.size(); }


  // remove 1 list element
  void remove(const insert_token& conn) {
    auto m_pos = conn.first;
    auto l_pos = conn.second;

    auto g_pos = map_.find(m_pos->first);
    if (g_pos == map_.end())
      return;

    auto next_group_l_pos = getNextGroupListPosition(m_pos);
    if (std::distance(g_pos->second, next_group_l_pos) <= 1) {
      map_.erase(m_pos);
      eraseList(l_pos);
    } else if (m_pos->second == l_pos) {
      m_pos->second = list_.erase(l_pos);
    } else {
      eraseList(l_pos);
    }
  }

  void remove_group(const group_key_type& group_key) {
    auto g_pos = map_.find(group_key);
    if (g_pos == map_.end())
      return;

    auto next_group_l_pos = getNextGroupListPosition(g_pos);
    eraseList(g_pos->second, next_group_l_pos);
    map_.erase(g_pos);
  }

  void remove_all() {
    list_.clear();
    map_.clear();
  }

  weak_vector getWeakList() const {
    weak_vector wvec;
    wvec.reserve(size());

//    for (auto it = list_.begin(); it != list_.end(); ++it) {
//      if (*it != nullptr)
//        wvec.emplace_back(*it);
//    }

    for (auto group_it = map_.begin(); group_it != map_.end(); ++group_it) {
      auto end = getNextGroupListPosition(group_it);
      for (auto list_it = group_it->second; list_it != end; ++list_it) {
        insert_token token{group_it, list_it};
        wvec.emplace_back(*list_it, token);
      }
    }

    return wvec;
//    for (auto it = list_.begin(); it != list_.end(); ++it) {
//      wvec.emplace_back(*it, {});
//    }
//
//    std::copy(list_.begin(), list_.end(), std::back_inserter(wvec));
//    return wvec;
  }

 private:
  void eraseList(list_iterator begin, list_iterator end) {
    list_.erase(begin, end);
  }

  void eraseList(list_iterator it) {
    if (it == list_.end())
      return;

    list_.erase(it);
  }

  list_iterator getNextGroupListPosition(map_const_iterator m_pos) const {
    auto next_group_m_pos = std::next(m_pos);
    if (next_group_m_pos == map_.end())
      return list_.end();
    return next_group_m_pos->second;
  }

  list_iterator getListIterator(const map_iterator& m_pos) {
    if (m_pos == map_.end())
      return list_.end();
    return m_pos->second;
  }

  list_iterator groupBegin(group_type group) {
    auto pos = map_.find(group);
    if (pos == map_.end())
      return list_.end();
    return pos->second;
  }

  list_iterator groupBeginNoCheck(map_iterator it) const {
    return it->second;
  }

  list_iterator groupEndNoCheck(map_iterator it) const {
    auto next_group_map_it = std::next(it);
    if (next_group_map_it == map_.end())
      return list_.end();
    return next_group_map_it->second;
  }

  mutable list list_;
  mutable map map_;
};

} // namespace experimental
} // namespace signal

# endif //VCCC_EXPERIMENTAL_SIGNAL_SLOT_GROUP_H_
