#include <vector>
#include <algorithm>
#include <iostream>
#include <list>

//Possible implementation of exchange 
template<class T, class U = T>
constexpr // since C++20
T myexchange(T& obj, U&& new_value)
    noexcept( // since C++23
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_assignable<T&, U>::value
    )
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

int main()
{
    // NEXT
    std::vector<int> v {3,2,3,4,5};
    std::cout << std::is_sorted(std::begin(v)  + 1, std::end(v)) << "\n";

    // In list we cannot increment iterator only with next
    std::list<int> l {3,2,3,4,5};
    std::cout << std::is_sorted(std::next(std::begin(v)  + 1), std::end(v)) << "\n";

    //EXCHANGE
    // possible implementation above
    // Moves old value to old value to return 
    // New value goes to obj
    // When copying is not efficient
    int a = 3;
    int b = std::exchange(a, 10);
    //int b = myexchange(a, 10);
    std::cout << a << " " << b << "\n";
    return 0;
}