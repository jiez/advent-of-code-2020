#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>

static bool is_valid(std::vector<unsigned long long>& input, int preamble, int index)
{
    for (int i = index - preamble; i < index; i++)
        for (int j = i + 1; j < index; j++)
            if (input[i] != input[j] && input[i] + input[j] == input[index])
                return true;
    return false;
}

int solution_for_puzzle_1(std::vector<unsigned long long>& input, int preamble)
{
    for (int i = preamble; i < input.size(); i++)
        if (!is_valid(input, preamble, i))
            return input[i];

    return 0;
}

int solution_for_puzzle_2(std::vector<unsigned long long>& input, unsigned long long invalid_num)
{
    int num = input.size();
    unsigned long long sums[num][num];

    memset(sums, 0, num * num * sizeof(unsigned long long));

    for (int i = 0; i < num; i++)
        sums[i][0] = input[i];

    for (int length = 2; length <= num; length++)
        for (int i = 0; i < num - length + 1; i++) {
            sums[i][length - 1] = input[i] + sums[i + 1][length - 2];
            if (sums[i][length - 1] == invalid_num) {
                unsigned long long min = input[i];
                unsigned long long max = input[i];
                for (int j = i + 1; j < i + length; j++) {
                    min = std::min(min, input[j]);
                    max = std::max(max, input[j]);
                }
                return min + max;
            }
        }

    return 0;
}

int main()
{
    std::vector<unsigned long long> input{};
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        unsigned long long n;
        is >> n;
        input.push_back(n);
    }

    unsigned long long first_invalid_num = solution_for_puzzle_1(input, 25);
    std::cout << "first invalid number is " << first_invalid_num << "\n";
    assert(first_invalid_num == 756008079);

    unsigned long long encryption_weakness = solution_for_puzzle_2(input, first_invalid_num);
    std::cout << "encryption weakness is " << encryption_weakness << "\n";
    assert(encryption_weakness == 93727241);

    return 0;
}
