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

static int solution_for_puzzle_1(const std::vector<std::string>& input, std::map<std::pair<int, int>, bool>& tiles)
{
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

static void add_more_tiles(std::map<std::pair<int, int>, bool>& tiles, int x_min, int x_max, int y_min, int y_max)
{
    for (int x = x_min; x <= x_max; x++)
        for (int y = y_min; y <= y_max; y++) {
            // not every (x, y) are valid
            // y's step is 1
            // when y is even number, x is even number
            // when y is odd number, x is odd number
            if (y % 2 == 0 && x % 2 == 1)
                continue;
            if (y % 2 == 1 && x % 2 == 0)
                continue;

            std::pair<int, int> tile{x, y};
            if (tiles.find(tile) == tiles.end())
                tiles[tile] = false;
        }
}

static int black_neighbors(std::map<std::pair<int, int>, bool>& tiles, int x, int y)
{
    int black = 0;

    std::vector<std::pair<int, int>> neighbors{
        {x + 2, y},
        {x + 1, y - 1},
        {x - 1, y - 1},
        {x - 2, y},
        {x - 1, y + 1},
        {x + 1, y + 1},
    };

    for (auto neighbor: neighbors) {
        auto it = tiles.find(neighbor);
        if (it != tiles.end() && it->second)
            black++;
    }

    return black;
}

static int solution_for_puzzle_2(std::map<std::pair<int, int>, bool>& tiles, int times)
{
    int x_min, x_max, y_min, y_max;

    // init x_min/max and y_min/max
    for (auto tile: tiles)
        if (tile.second) {
            x_min = x_max = tile.first.first;
            y_min = y_max = tile.first.second;
        }

    // find initial x_min/max and y_min/max
    for (auto tile: tiles)
        if (tile.second) {
            x_min = std::min(x_min, tile.first.first);
            x_max = std::max(x_max, tile.first.first);
            y_min = std::min(y_min, tile.first.second);
            y_max = std::max(y_max, tile.first.second);
        }

    for (int i = 0; i < times; i++) {
        add_more_tiles(tiles, x_min - 2, x_max + 2, y_min - 1, y_max + 1);

        std::map<std::pair<int, int>, bool> temp;

        for (auto tile: tiles) {
            int num = black_neighbors(tiles, tile.first.first, tile.first.second);
            if (tile.second && (num == 0 || num > 2))
                temp[tile.first] = false;
            else if (! tile.second && num == 2) {
                temp[tile.first] = true;
                x_min = std::min(x_min, tile.first.first);
                x_max = std::max(x_max, tile.first.first);
                y_min = std::min(y_min, tile.first.second);
                y_max = std::max(y_max, tile.first.second);
            } else
                temp[tile.first] = tile.second;
        }

        tiles = std::move(temp);
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

    // black: true
    // white: false
    std::map<std::pair<int, int>, bool> tiles;

    int black_tiles;

    black_tiles = solution_for_puzzle_1(input, tiles);
    std::cout << "black tiles: " << black_tiles << "\n";

    black_tiles = solution_for_puzzle_2(tiles, 100);
    std::cout << "black tiles after 100 times: " << black_tiles << "\n";

    return 0;
}
