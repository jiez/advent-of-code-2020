#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <iterator>

static bool is_valid(std::vector<int>& input, int preamble, int index)
{
    for (int i = index - preamble; i < index; i++)
        for (int j = i + 1; j < index; j++)
            if (input[i] != input[j] && input[i] + input[j] == input[index])
                return true;
    return false;
}

int solution_for_puzzle_1(std::vector<int>& input, int preamble)
{
    for (int i = preamble; i < input.size(); i++)
        if (!is_valid(input, preamble, i))
            return input[i];

    return 0;
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

    // std::for_each(input.begin(), input.end(), [](int n){std::cout << n << "\n";});

    int first_invalid_num = solution_for_puzzle_1(input, 25);
    std::cout << "first invalid number is " << first_invalid_num << "\n";

    return 0;
}
