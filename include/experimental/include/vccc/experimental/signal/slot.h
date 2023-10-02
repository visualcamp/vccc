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
# include <vector>

namespace vccc {
namespace experimental {

template<typename Func>
class slot;

template<typename R, typename ...Args>
class slot<R(Args...)> {
 public:
  using return_type = R;
  using function = std::function<R(Args...)>;
  using weak_container = std::vector<std::weak_ptr<void>>;
  using locked_container = std::vector<std::shared_ptr<void>>;

  slot(slot const&) = default;
  slot(slot &&) = default;
  slot& operator=(slot const&) = default;
  slot& operator=(slot &&) = default;

  template<typename Func>
  slot(const Func& func)
    : func_(func) {}

  locked_container lock() const {
    locked_container locked;
    locked.reserve(tracked_objects_.size());

    for(const auto& w : tracked_objects_)
      locked.emplace_back(w.lock());
    return locked;
  }

  bool expired() const {
    for(const auto& w : tracked_objects_)
      if (w.expired())
        return true;
    return false;
  }

  template<typename ...U>
  R operator()(U&&... args) const {
    return func_(std::forward<U>(args)...);
  }

  template<typename ...U>
  R operator()(U&&... args) {
    return func_(std::forward<U>(args)...);
  }

  template<typename T>
  slot& track(const std::shared_ptr<T> target) {
    tracked_objects_.emplace_back(target);
    return *this;
  }

  slot& track(std::weak_ptr<void> target) {
    tracked_objects_.emplace_back(target);
    return *this;
  }

 private:
  function func_;
  weak_container tracked_objects_;
};

} // namespace experimental
} // namespace vccc

# endif // VCCC_EXPERIMENTAL_SIGNAL_SLOT_H_
