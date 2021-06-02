# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_EXPERIMENTAL_SIGNAL_CONNECTION_H_
# define VCCC_EXPERIMENTAL_SIGNAL_CONNECTION_H_
#
# include <atomic>
# include <memory>
# include <utility>
#
# include "vccc/experimental/signal/slot_group.h"

namespace vccc {
namespace experimental {
namespace signal {

struct connection_impl_base {
  virtual ~connection_impl_base() = default;
  virtual bool is_connected() const = 0;
  virtual void disconnect() = 0;
};

template<typename Group, typename Slot>
struct connection_impl : public connection_impl_base {
  using slot_list_type = grouped_slot_list<Group, Slot>;
  using group_type = typename slot_list_type::group_type;
  using slot_type = typename slot_list_type::slot_type;
  using token_type = typename slot_list_type::insert_token;

  connection_impl()
    : connected(false) {}

  explicit connection_impl(const std::shared_ptr<slot_list_type>& slot_list_ptr)
    : connected(false), slot_list(slot_list_ptr) {}

  explicit connection_impl(const std::shared_ptr<slot_list_type>& slot_list_ptr, token_type&& token)
    : slot_list(slot_list_ptr), connection_token(std::move(token)), connected(true) {}

  void disconnect() override {
    auto slot_ptr = slot_list.lock();
    if (slot_ptr == nullptr)
      return;

    bool state = true;
    if (!connected.compare_exchange_strong(state, false))
      return;

    slot_ptr->remove(connection_token);
    slot_list.reset();
  }

  bool is_connected() const override {
    return connected.load();
  }

 private:
  std::weak_ptr<slot_list_type> slot_list;
  token_type connection_token;
  std::atomic<bool> connected{false};
};

class connection {
 public:

  connection() = default;

  explicit connection(std::shared_ptr<connection_impl_base> ptr)
    : pimpl(std::move(ptr)) {}

  void disconnect() {
    pimpl->disconnect();
  }

  bool is_connected() const {
    return pimpl->is_connected();
  }

 private:
  std::shared_ptr<connection_impl_base> pimpl;
};

} // namespace signal
} // namespace experimental
} // namespace signal

# endif //VCCC_EXPERIMENTAL_SIGNAL_CONNECTION_H_
