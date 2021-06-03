# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_EXPERIMENTAL_SIGNAL_SIGNAL_H_
# define VCCC_EXPERIMENTAL_SIGNAL_SIGNAL_H_
#
# include <functional>
# include <memory>
# include <mutex>
# include <type_traits>
# include <utility>
#
# include "vccc/experimental/signal/connection.h"
# include "vccc/experimental/signal/forward_declare.h"
# include "vccc/experimental/signal/slot_group.h"
#
# include "vccc/optional.hpp"

namespace vccc {
namespace experimental {

//class signal_impl_base {
// public:
//  virtual void disconnect()
//};

template<typename R, typename ...Args, typename Group>
class signal_impl<R(Args...), Group> :
  public std::enable_shared_from_this<signal_impl<R(Args...), Group>> {
  template<typename T>
  using is_void_return_type = typename std::is_void<R>::type;
  using void_return_type = std::true_type;
  using not_void_return_type = std::false_type;

 public:
  using slot_type = std::function<R(Args...)>;
  using group_type = Group;
  using return_type = std::conditional_t<std::is_same<void, R>::value, void, vccc::optional<R>>;
  using slot_list_type = grouped_slot_list<group_type, slot_type>;
  using group_key_type = typename slot_list_type::group_key_type;
  using weak_slot_list = typename slot_list_type::weak_vector;
  using weak_iterator = typename weak_slot_list::iterator;
  using token_type = typename slot_list_type::insert_token;
  using connection_body = connection_impl<signal_impl<R(Args...), Group>, token_type>;

  friend connection_body;

  enum {
    arity = sizeof...(Args),
  };

  signal_impl()
    : slot_list_(std::make_shared<slot_list_type>()) {}

  connection connect(const slot_type& slot, slot_position pos) {
    std::lock_guard<std::mutex> lck(slot_mutex_);

    std::shared_ptr<connection_impl_base> new_connection
      = make_new_connection(slot_list_->insert(std::make_shared<slot_type>(slot), pos));

//    connection conn(new_connection);
//    slot.autoDisconnectIfTracking(conn);
    return connection(new_connection);
  }

  connection connect(group_type group, const slot_type& slot, slot_position pos) {
    std::lock_guard<std::mutex> lck(slot_mutex_);

    std::shared_ptr<connection_impl_base> new_connection
      = make_new_connection(slot_list_->insert(group, std::make_shared<slot_type>(slot), pos));

//    connection conn(new_connection);
//    slot.autoDisconnectIfTracking(conn);
    return connection(new_connection);
  }

//  void disconnect(connection& conn) const {
//    conn.disconnect();
//  }
//
//  void disconnect(connection& conn) {
//    conn.disconnect();
//  }

  void disconnect(group_type group) {
    std::lock_guard<std::mutex> lck(slot_mutex_);

    group_key_type group_key(grouped, group);
    slot_list_->remove_group(group_key);
  }

  void disconnect(group_category category) {
    if (category == grouped)
      return;

    std::lock_guard<std::mutex> lck(slot_mutex_);
    group_key_type group_key(category);
    slot_list_->remove_group(group_key);
  }

  auto size() const {
    return slot_list_->size();
  }

  template<typename ...U>
  return_type operator()(U&&... args) {
    return invoke(is_void_return_type<return_type>{}, std::forward<U>(args)...);
  }

  template<typename ...U>
  return_type operator()(U&&... args) const {
    return invoke(is_void_return_type<return_type>{}, std::forward<U>(args)...);
  }

 private:

  void disconnect(connection_body& cbd, const token_type& token) {
    std::lock_guard<std::mutex> lck(slot_mutex_);
    slot_list_->remove(token);
  }

  void set_track(connection* conn, token_type* token, std::weak_ptr<void> target) {
    std::lock_guard<std::mutex> lck(slot_mutex_);
    **(token->second) = [conn = *conn, target = std::move(target), func = **(token->second)](Args&&... args) -> return_type {
      auto target_lock = target.lock();
      if (target_lock == nullptr) {
        conn.disconnect();
        return std::conditional_t<std::is_void<return_type>::value, void, return_type>();
      }
      return func(std::forward<Args>(args)...);
    };
  }

  std::shared_ptr<connection_body> make_new_connection(token_type&& token) {
    return std::make_shared<connection_body>(this->shared_from_this(), std::move(token));
  }

  template<typename ...U>
  void invoke(void_return_type, U&&... args) const {
    weak_slot_list weak_slots;
    {
      std::lock_guard<std::mutex> lck(slot_mutex_);
      weak_slots = slot_list_->getWeakList();
    }
    for (const auto& weak_ptr : weak_slots) {
      auto lock_ptr = weak_ptr.lock();
      if (lock_ptr != nullptr)
        (*lock_ptr)(args...);
    }
  }

  template<typename ...U>
  return_type invoke(not_void_return_type, U&&... args) const {
    weak_slot_list weak_slots;
    {
      std::lock_guard<std::mutex> lck(slot_mutex_);
      weak_slots = slot_list_->getWeakList();
    }
    return_type result;
    for (const auto& weak_ptr : weak_slots) {
      auto lock_ptr = weak_ptr.lock();
      if (lock_ptr != nullptr)
        result = (*lock_ptr)(args...);
    }
    return result;
  }

  std::shared_ptr<slot_list_type> slot_list_;
  mutable std::mutex slot_mutex_;
};


template<typename R, typename ...Args, typename Group>
class signal<R(Args...), Group> {
  using impl = signal_impl<R(Args...), Group>;
 public:
  using slot_type = typename impl::slot_type;
  using group_type = Group;
  using return_type = typename impl::return_type;

  signal()
    : pimpl_(std::make_shared<impl>()) {}

  connection connect(const slot_type& slot, slot_position pos = at_back) {
    return pimpl_->connect(slot, pos);
  }

  connection connect(group_type group, const slot_type& slot, slot_position pos = at_back) {
    return pimpl_->connect(group, slot, pos);
  }

//  void disconnect(connection& conn) const {
//    return pimpl_->disconnect(conn);
//  }
//
//  void disconnect(connection& conn) {
//    return pimpl_->disconnect(conn);
//  }

  void disconnect(group_type group) {
    return pimpl_->disconnect(group);
  }

  void disconnect(group_category group_cat) {
    return pimpl_->disconnect(group_cat);
  }

  auto size() const {
    return pimpl_->size();
  }

  template<typename ...U>
  return_type operator()(U&&... args) {
    return (*pimpl_)(std::forward<U>(args)...);
  }

  template<typename ...U>
  return_type operator()(U&&... args) const {
    return (*pimpl_)(std::forward<U>(args)...);
  }

 private:
  std::shared_ptr<impl> pimpl_;
};

} // namespace experimental
} // namespace signal

# endif //VCCC_EXPERIMENTAL_SIGNAL_SIGNAL_H_
