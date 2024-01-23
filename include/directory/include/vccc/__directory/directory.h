# /*
#  * Created by YongGyu Lee on 2021/06/02.
#  */
#
# ifndef VCCC_DIRECTORY_DIRECTORY_H_
# define VCCC_DIRECTORY_DIRECTORY_H_
#
# include <filesystem>
# include <functional>
# include <memory>

namespace vccc {

//! @addtogroup directory
//! @{

class directory {
 public:
  using iterator = std::filesystem::directory_iterator;
  using recursive_iterator = std::filesystem::recursive_directory_iterator;
  using entry = std::filesystem::directory_entry;
  using path = std::filesystem::path;
  using options = std::filesystem::directory_options;

 private:
  struct di_base {
    virtual void for_each(std::function<void(const entry&)> f) const = 0;
    [[nodiscard]] virtual bool is_recursive() const = 0;
  };

  template<typename DirectoryIterator>
  struct di_concrete : public di_base {
    static_assert(!std::is_reference_v<DirectoryIterator>);
    static_assert(!std::is_volatile_v<DirectoryIterator>);
    static_assert(!std::is_const_v<DirectoryIterator>);

    explicit di_concrete(DirectoryIterator it) : it_(std::move(it)) {};

    void for_each(std::function<void(const entry&)> f) const override {
      for(const auto& entry : it_) {
        f(entry);
      }
    }

    [[nodiscard]] bool is_recursive() const override {
      return std::is_same_v<DirectoryIterator, recursive_iterator>;
    }

    DirectoryIterator it_;
  };

  std::shared_ptr<di_base> ptr;

 public:
  directory()
    : ptr(make_di()) {}

  explicit directory(const path& path_, bool recursive = false) {
    reset(recursive, path_);
  }

  directory(const path& path_, options options_,
            bool recursive = false) {
    reset(recursive, path_, options_);
  }

  directory(const path& path_, std::error_code& ec,
            bool recursive = false) {
    reset(recursive, path_, ec);
  }

  directory(const path& path_, options options_,
            std::error_code& ec, bool recursive = false) {
    reset(recursive, path_, options_, ec);
  }

  template<typename DirectoryIterator,
    std::enable_if_t<
      std::is_same_v<DirectoryIterator, iterator> ||
      std::is_same_v<DirectoryIterator, recursive_iterator>,
    int> = 0>
  explicit directory(DirectoryIterator it)
    : ptr(std::make_shared<di_concrete<std::decay_t<DirectoryIterator>>>(std::move(it)))
  {}

  template<typename DirectoryIterator,
    std::enable_if_t<
      std::is_same_v<std::decay_t<DirectoryIterator>, iterator> ||
      std::is_same_v<std::decay_t<DirectoryIterator>, recursive_iterator>,
    int> = 0>
  directory& operator=(DirectoryIterator it) {
    ptr = std::make_shared<di_concrete<std::decay_t<DirectoryIterator>>>(std::move(it));
    return *this;
  }

  template<typename F>
  void for_each(F func) const {
    ptr->for_each([&](const entry& entry) {
      func(entry);
    });
  }

  [[nodiscard]]
  bool is_recursive() const {
    return ptr->is_recursive();
  }

 private:
  template<typename ...Args>
  static std::shared_ptr<di_concrete<iterator>> make_di(Args&&... args) {
    return std::make_shared<di_concrete<iterator>>(iterator(std::forward<Args>(args)...));
  }

  template<typename ...Args>
  static std::shared_ptr<di_concrete<recursive_iterator>> make_rdi(Args&&... args) {
    return std::make_shared<di_concrete<recursive_iterator>>(recursive_iterator(std::forward<Args>(args)...));
  }

  template<typename ...Args>
  void reset(bool recursive, Args&&... args) {
    if (recursive)
      ptr = make_rdi(std::forward<Args>(args)...);
    else
      ptr = make_di(std::forward<Args>(args)...);
  }

};

//! @} directory

} // namespace vccc

#endif // VCCC_DIRECTORY_DIRECTORY_H_
