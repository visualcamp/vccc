# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_SIGNAL_CONNECTION_H_
# define VCCC_SIGNAL_CONNECTION_H_
#
# include <atomic>
# include <memory>
# include <utility>
#
# include "vccc/__signal/forward_declare.h"
# include "vccc/__signal/slot_group.h"

namespace vccc {

//! @addtogroup signal
//! @{

class connection;

//! @cond IGNORED
struct connection_impl_base {
  virtual ~connection_impl_base() = default;
  virtual bool is_connected() const = 0;
  virtual void disconnect() = 0;
  virtual void track(connection* conn, std::weak_ptr<void> target) = 0;
};

template<typename Signal, typename SlotPtr>
struct connection_impl : public connection_impl_base {
  using signal_type = Signal;
  using slot_ptr = SlotPtr;

  connection_impl()
    : connected(false) {}

  connection_impl(const std::shared_ptr<signal_type>& signal_ptr, SlotPtr&& slot)
    : signal_ptr_(signal_ptr), slot_(std::move(slot)), connected(true) {}

  // TODO(Tony): Optimize
  void disconnect() override {
    auto ptr = signal_ptr_.lock();
    if (ptr == nullptr)
      return;

    bool state = true;
    if (!connected.compare_exchange_strong(state, false))
      return;

    auto weak_slot = slot_.first;
    if (weak_slot.lock() == nullptr)
      return;

    ptr->disconnect(slot_.second);
  }

  bool is_connected() const override {
    return connected.load();
  }

  // WIP: use to signal::connect() only (slot != expired() must be guaranteed)
  // signal.connect(...).track(...);  // Good
  // std::move(conn).track(...);      // Bad
  void track(connection* conn, std::weak_ptr<void> target) override {
    auto signal = signal_ptr_.lock();
    if (signal == nullptr)
      return;
    signal->set_track(conn, std::addressof(slot_.second), std::move(target));
  }

 private:
  std::weak_ptr<signal_type> signal_ptr_;
  slot_ptr slot_;
  std::atomic_bool connected{false};
};

//! @endcond IGNORED

/**
 @brief Connection manager for a single slot
 */
class connection {
 public:
  connection() = default;

  explicit connection(std::shared_ptr<connection_impl_base> ptr)
    : pimpl(std::move(ptr)) {}

  connection(connection const&) = default;
  connection(connection&&) noexcept = default;
  connection& operator=(connection const& other) = default;
  connection& operator=(connection&& other) noexcept = default;

  /**
   * Disconnect a given slot
   */
  void disconnect() const {
    if(pimpl == nullptr)
      return;
    pimpl->disconnect();
  }

  bool is_connected() const {
    return pimpl != nullptr && pimpl->is_connected();
  }

  connection track(std::weak_ptr<void> target) && {
    pimpl->track(this, std::move(target));
    return std::move(*this);
  }

 protected:
  std::shared_ptr<connection_impl_base> pimpl;
};

/**
 @brief Scoped connection manager for a single slot
 */
class raii_connection : connection {
  using base = connection;
 public:
  using base::is_connected;
  using base::disconnect;

  raii_connection() = default;
  ~raii_connection() {
    disconnect();
  }

  raii_connection(const raii_connection& other) = delete;
  raii_connection(raii_connection&& other) noexcept : base(other) {}
  raii_connection(const connection& conn) : base(conn) {}
  raii_connection(connection&& conn) noexcept : base(std::move(conn)) {}

  raii_connection& operator=(const raii_connection &rhs) = delete;
  raii_connection& operator=(raii_connection&& other) {
    if (this != std::addressof(other)) {
      disconnect();
      base::operator=(std::move(other));
    }
    return *this;
  }

  raii_connection& operator=(const connection& conn) {
    disconnect();
    base::operator=(conn);
    return *this;
  }

  raii_connection& operator=(connection&& conn) {
    if (this != std::addressof(conn)) {
      disconnect();
      base::operator=(std::move(conn));
    }
    return *this;
  }
};

//! @} addtogroup signal

} // namespace vccc

# endif // VCCC_SIGNAL_CONNECTION_H_
