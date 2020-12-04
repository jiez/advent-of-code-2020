#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>


int solution_for_puzzle_1(std::vector<std::map<std::string, std::string>> &passports)
{
    int valid_num = 0;

    for (auto passport : passports)
        if (passport.find("byr") == passport.end()
            || passport.find("iyr") == passport.end()
            || passport.find("eyr") == passport.end()
            || passport.find("hgt") == passport.end()
            || passport.find("hcl") == passport.end()
            || passport.find("ecl") == passport.end()
            || passport.find("pid") == passport.end()
            //|| passport.find("cid") == passport.end()
           )
            continue;
        else
            valid_num++;

    return valid_num;
}

int main()
{
    std::ifstream input_file{"input"};
    std::vector<std::map<std::string, std::string>> passports;

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    std::map<std::string, std::string> passport;
    while (getline(input_file, line)) {
        if (line.size() == 0) {
            std::map<std::string, std::string> temp = std::move(passport);
            passports.push_back(temp);
            continue;
        }
        std::istringstream is{line};
        std::string property;
        while (getline(is, property, ' ')) {
            //std::cout << property << "\n";
            std::size_t d = property.find(':');
            //std::cout << property.substr(0, d) << "=" <<  property.substr(d + 1) << "\n";
            passport.insert(std::pair<std::string, std::string>(property.substr(0, d), property.substr(d + 1)));
        }
    }
    passports.push_back(passport);

    /*
    for (auto passport : passports) {
        for (auto property : passport)
            std::cout << property.first << " @ " << property.second << "\n";
        std::cout << "\n";
    }
    */

    /*
    int total_properties = 0;
    for (auto passport : passports)
        total_properties += passport.size();
    std::cout << "total properties: " << total_properties << "\n";
    */

    int valid_num = solution_for_puzzle_1(passports);
    std::cout << "The number of valid passports: " << valid_num << "\n";

    return 0;
}
