# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_EXPERIMENTAL_SIGNAL_SLOT_H_
# define VCCC_EXPERIMENTAL_SIGNAL_SLOT_H_
#
# include <functional>
# include <memory>
# include <utility>
#
# include "vccc/experimental/signal/connection.h"

namespace vccc {
namespace experimental {

template<typename Signature, typename Function = std::function<Signature>>
class slot;

template<typename R, typename ...Args, typename Function>
class slot<R(Args...), Function> {
 public:
  using function = Function;

  slot() = default;

  template<typename Func>
  slot(const Func& func)
    : func_(func) {}

  template<typename ...U>
  R operator()(U&&... args) const {
    return func_(std::forward<U>(args)...);
  }

  slot& track(std::weak_ptr<void> target) {
    conn_ = std::make_shared<connection>();
    func_ = [target = std::move(target), conn = conn_, func = func_](Args&&... args) -> R {
      auto lock = target.lock();
      if (lock == nullptr) {
        conn->disconnect();
        return {};
      }
      return func(std::forward<Args>(args)...);
    };
    track_ = true;
    return *this;
  }

  bool is_tracking() const { return track_; }

  void autoDisconnectIfTracking(const connection& conn) const {
    if (is_tracking())
      *conn_ = conn;
  }

  function slot_function() const & { return func_; }
  function slot_function() const && { return std::move(func_); }

 private:
  function func_;
  bool track_ = false;
  mutable std::shared_ptr<connection> conn_;
};

} // namespace experimental
} // namespace vccc

# endif // VCCC_EXPERIMENTAL_SIGNAL_SLOT_H_
