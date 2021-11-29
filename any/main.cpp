#include <experimental/any>
#include <vector>
#include <string>
#include <iostream>

// Note: we ought to compile normally but launch it like this ".\a.exe | c++filt -t"
struct  S
{
    S() = default;
    // Copy constructor while adding to vector of any cannot be delete
    //S(const S &a) = delete;
};


int main()
{
    // Can store any value
    std::experimental::any a = 5;

    // Vector storing any tape o data
    std::vector<std::experimental::any> v{5, 4.2, std::string("Hello world")};

    // Casting with experimental namespace 
    std::cout << std::experimental::any_cast<int> (v[0]) << "\n";
    std::cout << std::experimental::any_cast<double> (v[1]) << "\n";

    // Deducting type of data
    std::cout << v[0].type().name() << "\n";
    std::cout << v[1].type().name() << "\n";
    std::cout << v[2].type().name() << "\n";

    // There is rqquriement to have copyconstructible object, copy constructor cannot be delete 
    v.push_back(S());

    // We can have casting in two forms 
    int *i = std::experimental::any_cast<int>(&v[0]);
    int j = std::experimental::any_cast<int>(v[0]);
    return 0;
}