#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

void solution_for_puzzle_1(std::vector<std::string> &map)
{
    // x pointing to right
    // y pointing down
    int x = 0, y = 0;
    int trees = 0;

    while (y < map.size()) {
        if (map[y][x] == '#')
            trees++;
        y++;
        x += 3;
        x %= map[0].size();
    }

    std::cout << "The number of trees: " << trees << "\n";
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

    solution_for_puzzle_1(map);

    return 0;
}
