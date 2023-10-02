# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_SIGNAL_SLOT_GROUP_H_
# define VCCC_SIGNAL_SLOT_GROUP_H_
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
# include "vccc/signal/group_key.h"

namespace vccc {

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
  using weak_slot = std::pair<weak_slot_ptr_type, insert_token>;
  using weak_slot_list = std::vector<weak_slot>;

  grouped_slot_list()
    : list_(), map_(group_key_compare_type {}) {}

  // insert grouped slot
  weak_slot insert(group_type group, slot_ptr_type slot, slot_position pos = at_back) {
    group_key_type key(grouped, group);
    if (pos == at_back)
      return insert_back(key, std::move(slot));
    else
      return insert_front(key, std::move(slot));
  }

  // insert ungrouped slot
  weak_slot insert(slot_ptr_type slot, slot_position pos = at_back) {
    group_key_type key;
    if (pos == at_back) {
      key.group_token.first = ungrouped_back;
    } else {
      key.group_token.first = ungrouped_front;
    }
    return insert_back(key, std::move(slot));
  }

  weak_slot insert_back(group_key_type group, slot_ptr_type slot) {
    auto group_it = map_.upper_bound(group); // find a upper bound of a group_key
    list_iterator list_it;

    if (group_it == map_.end()) {
      list_it = list_.emplace(list_.end(), std::move(slot));
      group_it = map_.emplace_hint(group_it, group, list_it);
    } else {
      list_it = list_.emplace(group_it->second, std::move(slot));
      if (group_it->first != group) { // if group does not exist
        group_it = map_.emplace_hint(group_it, group, list_it);
      }
    }

    return getWeakSlot(group_it, list_it);
  }

  weak_slot insert_front(group_key_type group, slot_ptr_type slot) {
    map_iterator group_it = map_.lower_bound(group); // find a lower bound of a group_key
    list_iterator list_it;

    if (group_it == map_.end()) {
      list_it = list_.emplace(list_.end(), std::move(slot));
      group_it = map_.emplace_hint(group_it, group, list_it);
    } else {
      if (group_it->first == group) { // if group exists
        list_it = list_.emplace(group_it->second, std::move(slot));
        group_it->second = list_it;
      } else { // group not exists
        list_it = list_.emplace(group_it->second, std::move(slot));
        group_it = map_.emplace_hint(group_it, group, list_it);
      }
    }

    return getWeakSlot(group_it, list_it);
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

  weak_slot getWeakSlot(map_iterator group, list_iterator index) const {
    return {*index, insert_token{group, index}};
  }

  weak_slot_list getWeakList() const {
    weak_slot_list weak_list;
    weak_list.reserve(size());

    for (auto group_it = map_.begin(); group_it != map_.end(); ++group_it) {
      auto list_end = getNextGroupListPosition(group_it);
      for (auto list_it = group_it->second; list_it != list_end; ++list_it) {
        weak_list.emplace_back(getWeakSlot(group_it, list_it));
      }
    }

    return weak_list;
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

} // namespace vccc

# endif //VCCC_SIGNAL_SLOT_GROUP_H_
