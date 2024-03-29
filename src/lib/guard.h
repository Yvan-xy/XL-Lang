#ifndef RJIT_GUARD_H
#define RJIT_GUARD_H

// reference: xstl https://github.com/MaxXSoft/XSTL

#include <utility>
#include <functional>

class Guard {
public:
  explicit Guard(std::function<void()> release) : release_(std::move(release)) {}

  // move constructor
  Guard(Guard &&guard) noexcept: release_(std::move(guard.release_)) {}

  // remove copy constructor
  Guard(const Guard &) = delete;

  // destructor
  ~Guard() { if (release_) release_(); }

  // remove move operator
  Guard &operator=(Guard &&) noexcept = delete;

  // remove copy operator
  Guard &operator=(const Guard &) = delete;

  // release manually
  void Release() {
    if (release_) {
      release_();
      release_ = nullptr;
    }
  }

private:
  std::function<void()> release_;
};

#endif //RJIT_GUARD_H
