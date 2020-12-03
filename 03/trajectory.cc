#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

int solution_for_puzzle_1(std::vector<std::string> &map, int right, int down)
{
    // x pointing to right
    // y pointing down
    int x = 0, y = 0;
    int trees = 0;

    while (y < map.size()) {
        if (map[y][x] == '#')
            trees++;
        x += right;
        y += down;
        x %= map[0].size();
    }

    return trees;
}

void solution_for_puzzle_2(std::vector<std::string> &map)
{
    std::vector<std::pair<int, int>> slopes =
        {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

    unsigned long long result = 1;

    for (auto slope: slopes) {
        int temp = solution_for_puzzle_1(map, slope.first, slope.second);
        // std::cout << slope.first << ", " << slope.second << ": " << temp << "\n";
        result *= temp;
    }

    std::cout << "The result is " << result << "\n";
}

int main()
{
    std::ifstream input_file{"input"};
    std::vector<std::string> map;

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        map.push_back(line);
    }

    //std::for_each(map.begin(), map.end(), [](std::string& line){std::cout << line << "\n";});

    int trees = solution_for_puzzle_1(map, 3, 1);
    std::cout << "The number of trees: " << trees << "\n";

    solution_for_puzzle_2(map);

    return 0;
}
