#include <coroutine>
#include <concepts>
#include <exception>
#include <iostream>
#include <cassert>

class resumable {
public:
    struct promise_type;
    resumable(std::coroutine_handle<promise_type> handle) : handle_(handle) { assert(handle); };
    resumable(resumable&) = delete;
    resumable(resumable&&) = delete;
    operator std::coroutine_handle<promise_type>() const {
        return handle_;
    }
    operator std::coroutine_handle<>() const {
        return handle_;
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

struct resumable::promise_type {
    resumable get_return_object() {
        return std::coroutine_handle<promise_type>::from_promise(*this);
    }
    std::suspend_never initial_suspend() { 
        return {};
    }
    std::suspend_never final_suspend() noexcept {
        return {};
    }
    void unhandled_exception() {}
};

struct Awaiter {
    std::coroutine_handle<> *handle_;
    bool await_ready() const noexcept { 
        std::cout << "Awaiting started ... \n";
        return false;
    }
    void await_suspend(std::coroutine_handle<> handle) {
        *handle_ = handle;
    }
    constexpr void await_resume() const noexcept {}
};

resumable counter(std::coroutine_handle<> *continuation_out)
{
    Awaiter awaiter{continuation_out};
    for (unsigned i = 0;; ++i) {
        co_await awaiter;
        std::cout << "Counter: " << i << "\n";
    }
}

int main(int argc, char *argv[])
{
    std::coroutine_handle<> co_handle;
    counter(&co_handle);
    for (int i = 0; i < 5; i++) {
        std::cout << "In main function\n";
        co_handle();
    }
    co_handle.destroy();
    return 0;
}