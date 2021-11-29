#include <typeinfo>
#include <cxxabi.h> 
#include <vector>
#include <list>
#include <iostream>
#include <string>

struct MyClass 
{
    std::vector<std::vector<std::list<std::pair<std::string, std::string>>>> m_data;
    std::vector<std::string> second_data;
};

std::string demangle(const char *mangled_name)
{
    int status;
    char *demangled_name = abi::__cxa_demangle(mangled_name, nullptr, 0, &status);
    if (status == 0) {
        std::string retval(demangled_name);
        free(demangled_name);
        return retval;
    } else {
        std::__throw_runtime_error("Error demangling");
    }
}

int main()
{
    MyClass a;
    auto name =  typeid(&MyClass::m_data).name();
    auto name2 = typeid(&MyClass::second_data).name();
    std::cout << "MANGLED NAME: " << name << '\n';
    std::cout << '\n';
    std::cout << "DEMANGLED NAME: " << demangle(name) << '\n';
    std::cout << "DEMANGLED NAME: " << demangle(name2) << '\n';
}
