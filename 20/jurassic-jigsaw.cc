#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


struct match_t {
    int tile_id;
    int side_id;
    bool flipped;
    match_t(int tid, int sid, bool flpd = false): tile_id{tid}, side_id{sid}, flipped{flpd} { }
};

struct side_t {
    std::string line;
    std::vector<match_t> matches;
    side_t(std::string l): line{l} { }
};

struct tile_t {
    int id;
    std::vector<std::string> img;
    std::vector<side_t> sides;
    std::vector<std::string> reversed_sides;
};

static void print_tile(const tile_t& tile)
{
    std::cout << "Tile " << tile.id << ":\n";
    for (auto line: tile.img)
        std::cout << line << "\n";
    /*
    for (auto side: tile.sides)
        std::cout << side.line << "  ";
    std::cout << "\n";

    for (auto side: tile.reversed_sides)
        std::cout << side << "  ";
    std::cout << "\n";
    */

    std::cout << "\n";
}

static unsigned long long solution_for_puzzle_1(std::vector<tile_t>& tiles)
{
    for (int i = 0; i < tiles.size(); i++)
        for (int j = i + 1; j < tiles.size(); j++)
            for (int s = 0; s < 4; s++)
                for (int t = 0; t < 4; t++) {
                    if (tiles[i].sides[s].line.compare(tiles[j].sides[t].line) == 0) {
                        tiles[i].sides[s].matches.push_back(match_t(tiles[j].id, t));
                        tiles[j].sides[t].matches.push_back(match_t(tiles[i].id, s));
                    }
                    if (tiles[i].sides[s].line.compare(tiles[j].reversed_sides[t]) == 0) {
                        tiles[i].sides[s].matches.push_back(match_t(tiles[j].id, t, true));
                        tiles[j].sides[t].matches.push_back(match_t(tiles[i].id, t, true));
                    }
                }

    /*
    for (auto tile: tiles) {
        std::cout << "Tile " << tile.id << ":\n";
        for (int s = 0; s < 4; s++) {
            std::cout << "  side " << s << " matches: ";
            for (auto match: tile.sides[s].matches) {
                std::cout << match.tile_id << "[" << match.side_id << "]";
                if (match.flipped)
                    std::cout << "(flipped), ";
                else
                    std::cout << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    */

    unsigned long long result = 1;

    for (auto tile: tiles) {
        int unmatched_sides = 0;
        for (auto side: tile.sides)
            if (side.matches.empty())
                unmatched_sides++;

        assert(unmatched_sides <= 2);

        if (unmatched_sides == 2) {
            // std::cout << "Tile " << tile.id << " num of unmatches sides: " << unmatched_sides << "\n";
            result *= tile.id;
        }
    }

    return result;
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<tile_t> tiles;

    std::string line;
    tile_t tile;
    while (getline(input_file, line)) {
        if (line == "") {
            // calculate 4 sides of image
            //
            //  side 0
            // s      s
            // i      i
            // d      d
            // e      e
            //
            // 3      1
            //  side 2

            tile.sides.push_back(side_t(tile.img[0]));

            std::string side1;
            for (auto line: tile.img)
                side1.push_back(line.back());
            tile.sides.push_back(side_t(side1));

            tile.sides.push_back(side_t(tile.img.back()));

            std::string side3;
            for (auto line: tile.img)
                side3.push_back(line[0]);
            tile.sides.push_back(side_t(side3));

            // reversed sides
            for (auto side: tile.sides) {
                std::reverse(side.line.begin(), side.line.end());
                tile.reversed_sides.push_back(side.line);
            }

            // add tile to tiles
            tiles.push_back(std::move(tile));

            continue;
        }

        if (line.compare(0, 4, "Tile") == 0) {
            int tile_id = std::stoi(line.substr(5));
            tile.id = tile_id;
            continue;
        }

        tile.img.push_back(line);
    }

    /*
    for (auto tile: tiles) {
        print_tile(tile);
        std::cout << "\n";
    }
    */

    unsigned long long result;

    result = solution_for_puzzle_1(tiles);
    std::cout << "result is " << result << "\n";

    return 0;
}
