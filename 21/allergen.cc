#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <cstdlib>
#include <algorithm>
#include <iterator>

static int solution_for_puzzle_1(std::map<std::string, std::vector<std::set<std::string>>>& allergens, std::map<std::string, int> all_ingredients)
{
    std::map<std::string, std::set<std::string>> possible_causes;

    for (auto allergen: allergens) {
        std::set<std::string> temp = allergen.second[0];
        for (int i = 1; i < allergen.second.size(); i++) {
            std::set<std::string> common_ingredients;
            std::set_intersection(temp.begin(), temp.end(),
                                  allergen.second[i].begin(), allergen.second[i].end(),
                                  std::inserter(common_ingredients, common_ingredients.begin()));
            temp = std::move(common_ingredients);
        }
        possible_causes[allergen.first] = temp;
    }

    /*
    for (auto allergen: possible_causes) {
        std::cout << allergen.first << ":";
        for (auto ingredient: allergen.second)
            std::cout << " " << ingredient;
        std::cout << "\n";
    }
    */

    std::set<std::string> known;

    for (auto allergen: possible_causes)
        if (allergen.second.size() == 1) {
            auto it = allergen.second.begin();
            known.insert(*it);
        }

    while (!known.empty()) {
        auto it = known.begin();
        auto ingredient = *it;
        known.erase(it);
        for (auto& allergen: possible_causes)
            if (allergen.second.size() > 1) {
                auto found = allergen.second.find(ingredient);
                if (found != allergen.second.end())
                    allergen.second.erase(found);

                if (allergen.second.size() == 1) {
                    auto ii = allergen.second.begin();
                    known.insert(*ii);
                }
            }
    }

    /*
    for (auto allergen: possible_causes) {
        std::cout << allergen.first << ":";
        for (auto ingredient: allergen.second)
            std::cout << " " << ingredient;
        std::cout << "\n";
    }
    */

    std::set<std::string> allergy_ingredients;
    for (auto allergen: possible_causes)
        for (auto ingredient: allergen.second)
            allergy_ingredients.insert(ingredient);

    int num = 0;
    for (auto ingredient: all_ingredients)
        if (allergy_ingredients.find(ingredient.first) == allergy_ingredients.end())
            num += ingredient.second;

    return num;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::map<std::string, std::vector<std::set<std::string>>> allergens;
    std::map<std::string, int> all_ingredients;

    std::regex name{"([a-z]+)"};
    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is(line);
        std::set<std::string> ingredients;
        std::smatch m;
        std::string s = line;
        while(std::regex_search(s, m, name)) {
            if (m[1] != "contains") {
                ingredients.insert(m[1]);
                if (all_ingredients.find(m[1]) == all_ingredients.end())
                    all_ingredients[m[1]] = 1;
                else
                    all_ingredients[m[1]]++;
                s = m.suffix().str();
            } else {
                s = m.suffix().str();
                break;
            }
        }

        /*
        std::cout << "Ingredients: ";
        for (auto ingredient: ingredients)
            std::cout << ingredient << " ";
        std::cout << "\n";
        */

        //std::cout << "Allergens: ";
        while(std::regex_search(s, m, name)) {
            std::string allergen = m[1];
            //std::cout << allergen << " ";

            if (allergens.find(allergen) == allergens.end()) {
                std::vector<std::set<std::string>> temp{ingredients};
                allergens[allergen] = temp;
            } else
                allergens[allergen].push_back(ingredients);

            s = m.suffix().str();
        }
        //std::cout << "\n";
    }

    int result = solution_for_puzzle_1(allergens, all_ingredients);
    std::cout << "result is " << result << "\n";

    return 0;
}
