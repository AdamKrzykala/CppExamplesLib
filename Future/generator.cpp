#include <iostream>
#include <random>
#include <iterator>
#include <algorithm>
#include <functional>
#include <set>
#include <future>

std::set<int> make_sorted_random(const size_t num_elements)
{
    std::set<int> retval;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elements-1);

    std::generate_n(std::inserter(retval, retval.end()), num_elements, [&](){ return dis(gen); });

    return retval;
}

int main()
{
    // Future value which can be obtain asynchronously in the future by using get()
    // std::launch::async       forces to parallelism
    // std::launch::deferred    not
    using myFuture = std::future<std::set<int>>;
    myFuture future1 = std::async(std::launch::async, make_sorted_random, 1000000);

    auto future2 = std::async(std::launch::async, make_sorted_random, 1000000);
    std::cout << future1.get().size() << " " << future2.get().size() << "\n";
    return 0;
}