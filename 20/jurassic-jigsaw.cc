#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <array>
#include <algorithm>

struct image_t {
    std::vector<std::string> img;

    // rotate 90 degree clockwise
    void rotate() {
        std::vector<std::string> temp;
        for (int i = 0; i < img[0].size(); i++) {
            std::string row;
            for (int j = img.size() - 1; j >= 0; j--)
                row.push_back(img[j][i]);
            temp.push_back(std::move(row));
        }
        img = std::move(temp);
    }

    // flip upside down
    void flip_updown() {
        std::reverse(img.begin(), img.end());
    }

    // flip left right
    void flip_leftright() {
        for (auto& row: img)
            std::reverse(row.begin(), row.end());
    }
};

struct sea_monster_t: public image_t {
    sea_monster_t() {
        img.push_back("                  # ");
        img.push_back("#    ##    ##    ###");
        img.push_back(" #  #  #  #  #  #   ");
    }
};

struct tile_t: public image_t {
    std::array<int, 4> neighbors = {-1, -1, -1, -1};

    tile_t() {}

    bool match(tile_t& other, int side) {
        for (int rotate = 0; rotate < 4; rotate++) {
            if (side == 0) {
                std::string other_side = other.img.back();
                if (img.front().compare(other_side) == 0)
                    return true;
                std::reverse(other_side.begin(), other_side.end());
                if (img.front().compare(other_side) == 0) {
                    other.flip_leftright();
                    return true;
                }
            }
            else if (side == 1) {
                bool matched = true;
                for (int i = 0; i < img.size(); i++)
                    if (img[i].back() != other.img[i].front()) {
                        matched = false;
                        break;
                    }
                if (matched)
                    return true;

                matched = true;
                for (int i = 0; i < img.size(); i++)
                    if (img[i].back() != other.img[img.size() - i - 1].front()) {
                        matched = false;
                        break;
                    }
                if (matched) {
                    other.flip_updown();
                    return true;
                }
            }
            else if (side == 2) {
                std::string other_side = other.img.front();
                if (img.back().compare(other_side) == 0)
                    return true;
                std::reverse(other_side.begin(), other_side.end());
                if (img.back().compare(other_side) == 0) {
                    other.flip_leftright();
                    return true;
                }
            }
            else if (side == 3) {
                bool matched = true;
                for (int i = 0; i < img.size(); i++)
                    if (img[i].front() != other.img[i].back()) {
                        matched = false;
                        break;
                    }
                if (matched)
                    return true;

                matched = true;
                for (int i = 0; i < img.size(); i++)
                    if (img[i].front() != other.img[img.size() - i - 1].back()) {
                        matched = false;
                        break;
                    }
                if (matched) {
                    other.flip_updown();
                    return true;
                }
            }

            other.rotate();
        }

        return false;
    }

};

static void print_tile(int id, const tile_t& tile)
{
    std::cout << "Tile " << id << ":\n";
    for (auto line: tile.img)
        std::cout << line << "\n";
    std::cout << "\n";
}

static unsigned long long solution_for_puzzle_1(std::map<int, tile_t>& tiles)
{
    std::set<int> processed;
    std::stack<int> working;

    // add an arbitary tile to working stack
    working.push(tiles.begin()->first);

    while(working.size() > 0) {
        int id = working.top();
        working.pop();
        auto it = tiles.find(id);
        // try to find a match tile for each side
        // side 0
        if (it->second.neighbors[0] == -1)
            for (auto ii = tiles.begin(); ii != tiles.end(); ++ii)
                if (it->second.match(ii->second, 0)) {
                    it->second.neighbors[0] = ii->first;
                    ii->second.neighbors[2] = it->first;
                    if (processed.find(ii->first) == processed.end())
                        working.push(ii->first);
                    break;
                }
        // side 1
        if (it->second.neighbors[1] == -1)
            for (auto ii = tiles.begin(); ii != tiles.end(); ++ii)
                if (it->second.match(ii->second, 1)) {
                    it->second.neighbors[1] = ii->first;
                    ii->second.neighbors[3] = it->first;
                    if (processed.find(ii->first) == processed.end())
                        working.push(ii->first);
                    break;
                }
        // side 2
        if (it->second.neighbors[2] == -1)
            for (auto ii = tiles.begin(); ii != tiles.end(); ++ii)
                if (it->second.match(ii->second, 2)) {
                    it->second.neighbors[2] = ii->first;
                    ii->second.neighbors[0] = it->first;
                    if (processed.find(ii->first) == processed.end())
                        working.push(ii->first);
                    break;
                }
        // side 3
        if (it->second.neighbors[3] == -1)
            for (auto ii = tiles.begin(); ii != tiles.end(); ++ii)
                if (it->second.match(ii->second, 3)) {
                    it->second.neighbors[3] = ii->first;
                    ii->second.neighbors[1] = it->first;
                    if (processed.find(ii->first) == processed.end())
                        working.push(ii->first);
                    break;
                }

        processed.insert(it->first);
    }

    // find the top left corner tile
    unsigned long long top_left = tiles.begin()->first;
    auto it = tiles.find(top_left);
    while (it->second.neighbors[3] != -1) {
        top_left = it->second.neighbors[3];
        it = tiles.find(top_left);
    }
    while (it->second.neighbors[0] != -1) {
        top_left = it->second.neighbors[0];
        it = tiles.find(top_left);
    }

    unsigned long long top_right = top_left;
    it = tiles.find(top_right);
    while (it->second.neighbors[1] != -1) {
        top_right = it->second.neighbors[1];
        it = tiles.find(top_right);
    }

    unsigned long long bottom_left = top_left;
    it = tiles.find(bottom_left);
    while (it->second.neighbors[2] != -1) {
        bottom_left = it->second.neighbors[2];
        it = tiles.find(bottom_left);
    }

    unsigned long long bottom_right = top_right;
    it = tiles.find(bottom_right);
    while (it->second.neighbors[2] != -1) {
        bottom_right = it->second.neighbors[2];
        it = tiles.find(bottom_right);
    }

    return top_left * top_right * bottom_left * bottom_right;
}

static void search_sea_monster(const std::vector<std::string>& image, const std::vector<std::string>& monster, std::vector<std::string>& record)
{
    for (int i = 0; i < image.size() - monster.size(); i++)
        for (int j = 0; j < image[0].size() - monster[0].size(); j++) {
            bool found = true;
            for (int k = 0; k < monster.size(); k++) {
                for (int l = 0; l < monster[0].size(); l++)
                    if (monster[k][l] == '#' && image[i + k][j + l] != '#') {
                        found = false;
                        break;
                    }
                if (!found)
                    break;
            }
            if (found)
                for (int k = 0; k < monster.size(); k++)
                    for (int l = 0; l < monster[0].size(); l++)
                        if (monster[k][l] == '#')
                            record[i + k][j + l] = 'O';
        }
}

static unsigned long long solution_for_puzzle_2(std::map<int, tile_t>& tiles)
{
    std::vector<std::string> image;

    // find the top left corner tile
    int top_left = tiles.begin()->first;
    auto it = tiles.find(top_left);
    while (it->second.neighbors[3] != -1) {
        top_left = it->second.neighbors[3];
        it = tiles.find(top_left);
    }
    while (it->second.neighbors[0] != -1) {
        top_left = it->second.neighbors[0];
        it = tiles.find(top_left);
    }

    int next_row_first = top_left;

    while (next_row_first != -1) {
        int first_row = image.size();

        it = tiles.find(next_row_first);
        for (int i = 1; i < it->second.img.size() - 1; i++) {
            std::string line = it->second.img[i];
            image.push_back(line.substr(1, line.size() - 2));
        }

        next_row_first = it->second.neighbors[2];

        int id = it->second.neighbors[1];
        while (id != -1) {
            it = tiles.find(id);
            for (int i = 1; i < it->second.img.size() - 1; i++) {
                std::string line = it->second.img[i];
                image[first_row + i - 1].append(line.substr(1, line.size() - 2));
            }
            id = it->second.neighbors[1];
        }
    }

    // try to find the sea monsters in image
    sea_monster_t monster;
    std::vector<std::string> record = image;

    for (int i = 0; i < 4; i++) {
        search_sea_monster(image, monster.img, record);
        monster.rotate();
    }

    monster.flip_updown();

    for (int i = 0; i < 4; i++) {
        search_sea_monster(image, monster.img, record);
        monster.rotate();
    }

    int water_roughness = 0;

    for (int i = 0; i < image.size(); i++) {
        // std::cout << record[i] << "\n";
        for (int j = 0; j < image[0].size(); j++)
            if (image[i][j] == '#' && record[i][j] != 'O')
                water_roughness++;
    }

    return water_roughness;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::map<int, tile_t> tiles;

    std::string line;
    tile_t tile;
    int tile_id;
    while (getline(input_file, line)) {
        if (line == "") {
            // add tile to tiles
            tiles[tile_id] = std::move(tile);
            continue;
        }

        if (line.compare(0, 4, "Tile") == 0) {
            tile_id = std::stoi(line.substr(5));
            continue;
        }

        tile.img.push_back(line);
    }

    /*
    for (auto tile: tiles) {
        print_tile(tile.first, tile.second);
        std::cout << "\n";
    }
    */

    unsigned long long result;

    result = solution_for_puzzle_1(tiles);
    std::cout << "result is " << result << "\n";

    result = solution_for_puzzle_2(tiles);
    std::cout << "result is " << result << "\n";

    return 0;
}
