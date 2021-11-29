#include <iostream>
#include <fstream>
#include <chrono>

void writelnWithEndl(std::ostream &os, const std::string &str)
{
    //Endl is equivalent to \n and std::flush 
    os << str << std::endl;
}

void writelnFaster(std::ostream &os, const std::string &str)
{
    os << str << "\n";
}

int main()
{
    std::ofstream outfileSlower("outSlower.txt", std::ios_base::trunc);
    std::ofstream outfileFaster("outFaster.txt", std::ios_base::trunc);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        writelnWithEndl(outfileSlower, "Hello world");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << duration.count() << "\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        writelnFaster(outfileFaster, "Hello world");
    }
    outfileFaster << std::flush;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << duration.count() << "\n";
}