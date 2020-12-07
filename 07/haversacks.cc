#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <regex>


int solution_for_puzzle_1(std::unordered_map<std::string, std::unordered_map<std::string, int>>& bags, const std::string& start_bag)
{
    std::unordered_set<std::string> bag_set;
    std::unordered_set<std::string> working_set{start_bag};

    while (working_set.size() > 0) {
        std::unordered_set<std::string> new_set;
        for (auto b: working_set) {
            bag_set.insert(b);
            for (auto bb: bags)
                if (bb.second.find(b) != bb.second.end())
                    new_set.insert(bb.first);
        }

        working_set = std::move(new_set);
    }

    return bag_set.size() - 1;
}

static int required_bags(std::unordered_map<std::string, std::unordered_map<std::string, int>>& bags, const std::string& bag)
{
    int count = 1;

    for (auto b: bags[bag])
        count += required_bags(bags, b.first) * b.second;

    return count;
}

int solution_for_puzzle_2(std::unordered_map<std::string, std::unordered_map<std::string, int>>& bags, const std::string& start_bag)
{
    return required_bags(bags, start_bag) - 1;
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> bags;
    std::regex first_bag{"([a-z]+ [a-z]+) bags contain "};
    std::regex other_bag{"([0-9]+) ([a-z]+ [a-z]+) bag"};

    while (getline(input_file, line)) {
        //std::cout << line << "\n";
        std::smatch m;
        std::regex_search(line, m, first_bag);
        std::string first_color = m[1];

        std::unordered_map<std::string, int> other_bags;
        std::string s = m.suffix().str();

        while(std::regex_search(s, m, other_bag)) {
            other_bags[m[2]] = std::stoi(m[1]);
            s = m.suffix().str();
        }
        bags[first_color] = other_bags;
    }

    int how_many;
    std::string shiny_gold{"shiny gold"};

    how_many = solution_for_puzzle_1(bags, shiny_gold);
    std::cout << "The number of bags containing shiny gold: " << how_many << " (378 expected)\n";

    how_many = solution_for_puzzle_2(bags, shiny_gold);
    std::cout << "The number of bags required inside shiny gold bag: " << how_many << " (27526 expected)\n";

    return 0;
}
