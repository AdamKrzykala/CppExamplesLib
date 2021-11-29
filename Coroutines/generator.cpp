#include <coroutine>
#include <iostream>
#include <cassert>

template<typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    Generator(handle_type h) : coro(h) {}

    ~Generator() {
        if (coro) coro.destroy();
    }

    T getValue() {
        return coro.promise().current_value;
    }

    bool next() {
        coro.resume();
        return not coro.done();
    }
};

template<typename T>
struct Generator<T>::promise_type
{
    using coro_handle = std::coroutine_handle<promise_type>;

    auto get_return_object() {
        return coro_handle::from_promise(*this);
    }

    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }

    void return_void() {}

    auto yield_value(const T value) {
        current_value = value;
        return std::suspend_always();
    }

    void unhandled_exception() {
        std::terminate();
    }

    T current_value;
};

Generator<int> getNext(int start = 0, int step = 1) noexcept {
    auto value = start;
    for (int i = 0;; ++i) {
        co_yield value;
        value += step;
    }
}

int main() 
{
    auto gen1 = getNext();
    for (int i = 0; i < 10; i++) {
        gen1.next();
        std::cout << " " << gen1.getValue();
    }
    std::cout << "\n\n";

    auto gen2 = getNext(100, -20);
    for (int i = 0; i < 10; i++) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }
}