#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <bitset>

int solution_for_puzzle_1(std::vector<std::vector<std::bitset<26>>> &groups)
{
    int sum = 0;
    std::bitset<26> summary;

    for (auto group: groups) {
        summary.reset();

        for (auto person: group)
            summary |= person;

        sum += summary.count();
    }

    return sum;
}


int solution_for_puzzle_2(std::vector<std::vector<std::bitset<26>>> &groups)
{
    int sum = 0;
    std::bitset<26> summary;

    for (auto group: groups) {
        summary.set();

        for (auto person: group)
            summary &= person;

        sum += summary.count();
    }

    return sum;
}


int main()
{
    std::ifstream input_file{"input"};
    std::vector<std::vector<std::bitset<26>>> groups;

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    std::vector<std::bitset<26>> group;

    while (getline(input_file, line)) {
        if (line.size() == 0) {
            std::vector<std::bitset<26>> temp = std::move(group);
            groups.push_back(temp);
            continue;
        }
        std::bitset<26> questions;
        for (auto c: line)
            questions.set(c - 'a');
        group.push_back(questions);
    }
    groups.push_back(group);

    int result;
    result = solution_for_puzzle_1(groups);
    std::cout << "The sum for puzzle 1 is: " << result << " (6748 expected)" << "\n";

    result = solution_for_puzzle_2(groups);
    std::cout << "The sum for puzzle 2 is: " << result << " (3445 expected)" << "\n";

    return 0;
}
