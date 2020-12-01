#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

void solution_for_puzzle_1(std::vector<int> &input)
{
    for (auto it1 = input.begin(); it1 != input.end(); ++it1)
        for (auto it2 = it1 + 1; it2 != input.end(); ++it2)
            if (it1 != it2 && *it1 + *it2 == 2020) {
                std::cout << *it1 << " * " << *it2 << " = " << *it1 * *it2 << "\n";
                return;
            }
}

int main()
{
    std::vector<int> input{};
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        int n;
        is >> n;
        input.push_back(n);
    }

    solution_for_puzzle_1(input);

    // std::for_each(input.begin(), input.end(), [](int n){std::cout << n << "\n";});

    return 0;
}
