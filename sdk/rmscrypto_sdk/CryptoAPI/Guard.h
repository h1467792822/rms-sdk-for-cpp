#ifndef RMSCRYPTO_GUARD_H
#define RMSCRYPTO_GUARD_H

#include <functional>

namespace rmscrypto {
namespace api {
class Guard {
    std::function<void()> fun;
    mutable bool dismiss = false;
    Guard(const Guard&);
    Guard& operator=(const Guard&);

public:
    Guard(std::function<void()> fun) : fun(fun) {}
    ~Guard()
    {
        if (!dismiss) fun();
    }
    void Dismiss() const { dismiss = true; }
};

}    // namespace api
}    // namespace rmscrypto

#define GUARD_CONCAT_NAME(x, y) x##y
#define GUARD_VARIABLE(name) GUARD_CONCAT_NAME(name, __LINE__)
#define MAKE_GUARD(fun) \
    const ::rmscrypto::api::Guard& GUARD_VARIABLE(guard) = ::rmscrypto::api::Guard(std::function<void()>(fun))

#endif
