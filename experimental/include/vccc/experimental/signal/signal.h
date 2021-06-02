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
# include "vccc/experimental/signal/slot_group.h"
#
# include "vccc/optional.hpp"

namespace vccc {
namespace experimental {
namespace signal {

template<typename F, typename Group = int>
class signal;

template<typename R, typename ...Args, typename Group>
class signal<R(Args...), Group> {
  template<typename T>
  using is_void_return_type = typename std::is_void<R>::type;
  using void_return_type = std::true_type;
  using not_void_return_type = std::false_type;

 public:
  using slot_type = std::function<R(Args...)>;
  using group_type = Group;
  using return_type = std::conditional_t<std::is_same<void, R>::value, void, vccc::optional<R>>;
  using slot_list_type = grouped_slot_list<group_type, slot_type>;
  using weak_slot_list = typename slot_list_type::weak_vector;
  using weak_iterator = typename weak_slot_list::iterator;
  using connection_body = connection_impl<group_type, slot_type>;
  using token_type = typename slot_list_type::insert_token;

  enum {
    arity = sizeof...(Args),
  };

  signal()
    : slot_list_(std::make_shared<slot_list_type>()) {}

  connection connect(const slot_type& slot, position pos = at_back) {
    std::lock_guard<std::mutex> lck(slot_mutex_);

    std::shared_ptr<connection_impl_base> new_connection
      = make_new_connection(slot_list_->insert(std::make_shared<slot_type>(slot), pos));

    return connection(new_connection);
  }

  connection connect(group_type group, const slot_type& slot, position pos = at_back) {
    std::lock_guard<std::mutex> lck(slot_mutex_);

    std::shared_ptr<connection_impl_base> new_connection
      = make_new_connection(slot_list_->insert(group, std::make_shared<slot_type>(slot), pos));

    return connection(new_connection);
  }

  void disconnect(connection& conn) const {
    conn.disconnect();
  }

  void disconnect(connection& conn) {
    conn.disconnect();
  }

  void disconnect(group_type group) {
    slot_list_->remove(group);
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
  std::shared_ptr<connection_body> make_new_connection(token_type&& token) const {
    return std::make_shared<connection_body>(slot_list_, std::move(token));
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

} // namespace signal
} // namespace experimental
} // namespace signal

# endif //VCCC_EXPERIMENTAL_SIGNAL_SIGNAL_H_
