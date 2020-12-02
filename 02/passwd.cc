#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

struct passwd {
    int min;
    int max;
    char c;
    std::string pwd;
};

void solution_for_puzzle_1(std::vector<passwd> &input)
{
    int valid_count = 0;

    for (auto it = input.begin(); it != input.end(); ++it) {
        int num = 0;
        for (auto c: it->pwd)
            if (c == it->c)
                num++;
        if (num >= it->min && num <= it->max)
            valid_count++;
    }

    std::cout << "The number of valid passwords for puzzle 1: " << valid_count << "\n";
}

void solution_for_puzzle_2(std::vector<passwd> &input)
{
    int valid_count = 0;

    for (auto it = input.begin(); it != input.end(); ++it) {
        int num = 0;
        if (it->min <= it->pwd.size() && it->pwd[it->min - 1] == it->c)
            num++;
        if (it->max <= it->pwd.size() && it->pwd[it->max - 1] == it->c)
            num++;
        if (num == 1)
            valid_count++;
    }

    std::cout << "The number of valid passwords for puzzle 2: " << valid_count << "\n";
}

int main()
{
    // input looks like
    // 1-3 a: abcde
    // 1-3 b: cdefg

    std::vector<passwd> input{};
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        passwd pwd;
        char dummy;
        
        is >> pwd.min >> dummy >> pwd.max >> pwd.c >> dummy >> pwd.pwd;
        input.push_back(pwd);
    }

    //std::for_each(input.begin(), input.end(), [](passwd pwd){std::cout << pwd.min << "-" << pwd.max << " " << pwd.c << ": " << pwd.pwd << "\n";});

    solution_for_puzzle_1(input);

    solution_for_puzzle_2(input);

    return 0;
}
