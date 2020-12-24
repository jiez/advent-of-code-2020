#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


static std::pair<int, int> locate_tile(const std::string& line) {
    // x pointing east
    // y pointing north
    int x = 0;
    int y = 0;

    for (int i = 0; i < line.size();) {
        if (line[i] == 'e') {
            x += 2;
            i++;
        } else if (line[i] == 'w') {
            x -= 2;
            i++;
        } else if (line[i] == 's' && line[i + 1] == 'e') {
            x += 1;
            y -= 1;
            i += 2;
        } else if (line[i] == 's' && line[i + 1] == 'w') {
            x -= 1;
            y -= 1;
            i += 2;
        } else if (line[i] == 'n' && line[i + 1] == 'w') {
            x -= 1;
            y += 1;
            i += 2;
        } else if (line[i] == 'n' && line[i + 1] == 'e') {
            x += 1;
            y += 1;
            i += 2;
        } else
            assert(false);
    }

    return std::pair<int, int>{x, y};
}

static int solution_for_puzzle_1(const std::vector<std::string>& input)
{
    // black: true
    // white: false
    std::map<std::pair<int, int>, bool> tiles;

    for (auto line: input) {
        std::pair<int, int> tile = locate_tile(line);
        if (tiles.find(tile) == tiles.end())
            tiles[tile] = true;
        else
            tiles[tile] = ! tiles[tile];
    }

    int black = 0;
    for (auto tile: tiles)
        if (tile.second)
            black++;

    return black;
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<std::string> input;
    std::string line;

    while (getline(input_file, line))
        input.push_back(line);        

    int black_tiles;

    black_tiles = solution_for_puzzle_1(input);
    std::cout << "black tiles: " << black_tiles << "\n";

    return 0;
}
