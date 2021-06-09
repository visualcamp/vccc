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
# include "vccc/experimental/signal/forward_declare.h"
# include "vccc/experimental/signal/slot_group.h"

namespace vccc {
namespace experimental {

class connection;

struct connection_impl_base {
  virtual ~connection_impl_base() = default;
  virtual bool is_connected() const = 0;
  virtual void disconnect() = 0;
  virtual void track(connection* conn, std::weak_ptr<void> target) = 0;
};

template<typename Signal, typename Token>
struct connection_impl : public connection_impl_base {
  using signal_type = Signal;
  using token_type = Token;

  connection_impl()
    : connected(false) {}

  connection_impl(const std::shared_ptr<signal_type>& signal_ptr, token_type&& token)
    : signal_ptr_(signal_ptr), token_(std::move(token)), connected(true) {}

  void disconnect() override {
    auto ptr = signal_ptr_.lock();
    if (ptr == nullptr)
      return;

    bool state = true;
    if (!connected.compare_exchange_strong(state, false))
      return;

    ptr->disconnect(*this, token_);
  }

  bool is_connected() const override {
    return connected.load();
  }

  void track(connection* conn, std::weak_ptr<void> target) override {
    auto ptr = signal_ptr_.lock();
    if (ptr == nullptr)
      return;
    ptr->set_track(conn, &token_, std::move(target));
  }

 private:
  std::weak_ptr<signal_type> signal_ptr_;
  token_type token_;
  std::atomic_bool connected{false};
};

class connection {
 public:
  connection() = default;

  explicit connection(std::shared_ptr<connection_impl_base> ptr)
    : pimpl(std::move(ptr)) {}

  connection(connection const&) = default;
  connection(connection &&) = default;
  connection& operator=(connection const&) = default;

  connection& operator=(connection&& other) {
    disconnect();
    pimpl = std::move(other.pimpl);
    return *this;
  }

  void disconnect() const { if(pimpl) pimpl->disconnect(); }
  void disconnect() { if(pimpl) pimpl->disconnect(); }

  bool is_connected() const {
    return pimpl != nullptr && pimpl->is_connected();
  }

  connection& track(std::weak_ptr<void> target) && {
    pimpl->track(this, std::move(target));
    return *this;
  }

 protected:
  std::shared_ptr<connection_impl_base> pimpl;
};

class raii_connection : connection {
  using base = connection;
 public:
  raii_connection() = default;
  ~raii_connection() {
    this->disconnect();
  }

  raii_connection(connection&& conn)
    : base(std::move(conn)) {}

  raii_connection& operator=(connection&& conn) {
    base::operator=(std::move(conn));
    return *this;
  }

//  raii_connection& operator=(connection&& conn) {
//    base::operator=(std::move(conn));
//    return *this;
//  }
};

} // namespace experimental
} // namespace signal

# endif //VCCC_EXPERIMENTAL_SIGNAL_CONNECTION_H_
