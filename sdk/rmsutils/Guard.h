#ifndef RMSUTILS_GUARD_H
#define RMSUTILS_GUARD_H

#include <functional>

namespace rmsutils {

class Guard {
  std::function<void()> release;
  mutable bool dismiss = false;
  Guard(const Guard&);
  Guard(Guard&&);
  Guard& operator=(const Guard&);
public:
  Guard(std::function<void()> release): release(release) {}
  void Dismiss() const { dismiss = true; }
  ~Guard() {
    if (!dismiss) {
      release();
    }
  }
};

#define GUARD_CONCAT_NAME(x, y) x##y
#define GUARD_CONCAT(x, y) GUARD_CONCAT_NAME(x, y)
#define GUARD_VARIABLE(name) GUARD_CONCAT(name, __LINE__)
#define MAKE_GUARD(release) \
  const ::rmsutils::Guard& GUARD_VARIABLE(rmsutils_guard) = ::rmsutils::Guard(::std::function<void()>(release))

} // namespace rmsutils

#endif

