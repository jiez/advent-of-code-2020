#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int solution_for_puzzle_1(const std::vector<int>& adapters)
{
    int diff1 = 0;
    int diff3 = 0;

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

unsigned long long solution_for_puzzle_2(const std::vector<int>& adapters)
{
    int num = adapters.size();
    std::vector<unsigned long long> ways(num, 0);

    ways[num - 1] = 1;

    for (int i = num - 2; i >= 0; i--) {
        unsigned long long subways = 0;

        subways = ways[i + 1];

        if (i + 2 < num && adapters[i + 2] - adapters[i] <= 3)
            subways += ways[i + 2];

        if (i + 3 < num && adapters[i + 3] - adapters[i] <= 3)
            subways += ways[i + 3];

        ways[i] = subways;
    }

    return ways[0];
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

    adapters.push_back(0);
    std::sort(adapters.begin(), adapters.end());

    int result = solution_for_puzzle_1(adapters);
    std::cout << "result is " << result << "\n";
    assert(result == 1700);

    unsigned long long ways = solution_for_puzzle_2(adapters);
    std::cout << "There are " << ways << " ways\n";
    assert(ways == 12401793332096ULL);

    return 0;
}
