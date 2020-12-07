#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <regex>


int solution_for_puzzle_1(std::vector<std::vector<int>> &bags, int start_bag)
{
    std::unordered_set<int> bag_set;
    std::unordered_set<int> working_set{start_bag};

    while (working_set.size() > 0) {
        std::unordered_set<int> new_set;
        for (auto b: working_set) {
            bag_set.insert(b);
            for (int i = 0; i < bags.size(); i++)
                if (bags[i][b] > 0)
                    new_set.insert(i);
        }

        working_set = std::move(new_set);
    }

    return bag_set.size() - 1;
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    std::vector<std::vector<int>> bags;
    std::map<std::string, int> color2index;
    std::vector<std::string> colors;
    std::regex bag_color{"([a-z]+ [a-z]+) bags contain "};
    int color_index = 0;

    while (getline(input_file, line)) {
        //std::cout << line << "\n";
        std::smatch m;
        std::regex_search(line, m, bag_color);
        std::string color = m[1];
        colors.push_back(color);
        color2index.insert({color, color_index});
        color_index++;
    }

    int num_of_colors = color_index;
    input_file.clear();
    input_file.seekg(0, input_file.beg);
    std::regex bag_color2{"([0-9]+) ([a-z]+ [a-z]+) bag"};
    color_index = 0;

    while (getline(input_file, line)) {
        //std::cout << line << "\n";
        std::vector<int> containing_bags(num_of_colors, 0);
        std::smatch m;
        std::regex_search(line, m, bag_color);
        std::string s = m.suffix().str();

        while(std::regex_search(s, m, bag_color2)) {
            //std::cout << m[2] << ":" << color2index[m[2]] << ":" << m[1] << "\n";
            if (m[1].compare("no other") != 0)
                containing_bags[color2index[m[2]]] = std::stoi(m[1]);
            s = m.suffix().str();
        }
        bags.push_back(std::move(containing_bags));
    }

    int how_many;

    how_many = solution_for_puzzle_1(bags, color2index["shiny gold"]);
    std::cout << "The number of bags containing shiny gold: " << how_many << " (182 expected)\n";

/*
    valid_num = solution_for_puzzle_2(passports);
    std::cout << "The number of strictly valid passports: " << valid_num << " (109 expected)\n";
*/

    return 0;
}
