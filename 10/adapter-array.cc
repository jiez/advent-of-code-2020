#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int solution_for_puzzle_1(std::vector<int>& adapters)
{
    int diff1 = 0;
    int diff3 = 0;

    adapters.push_back(0);

    std::sort(adapters.begin(), adapters.end());

    for (int i = 0; i < adapters.size() - 1; i++) {
        int diff = adapters[i + 1] - adapters[i];
        assert(diff <= 3);
        if (diff == 1)
            diff1++;
        if (diff == 3)
            diff3++;
    }

    diff3++;

    return diff1 * diff3;
}

int main()
{
    std::vector<int> adapters{};
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
        adapters.push_back(n);
    }

    int result = solution_for_puzzle_1(adapters);
    std::cout << "result is " << result << "\n";

    return 0;
}
