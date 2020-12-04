#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <regex>


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
            || passport.find("pid") == passport.end())
            continue;
        else
            valid_num++;

    return valid_num;
}

bool check_year(std::string year, int min, int max)
{
    const std::regex year_regex{"[0-9]{4}"};
    if (! std::regex_match(year, year_regex))
        return false;

    int y = std::stoi(year);
    if (y >= min && y <= max)
        return true;
    else
        return false;
}

int solution_for_puzzle_2(std::vector<std::map<std::string, std::string>> &passports)
{
    int valid_num = 0;

    for (auto passport : passports) {
        if (passport.find("byr") == passport.end()
            || passport.find("iyr") == passport.end()
            || passport.find("eyr") == passport.end()
            || passport.find("hgt") == passport.end()
            || passport.find("hcl") == passport.end()
            || passport.find("ecl") == passport.end()
            || passport.find("pid") == passport.end())
            continue;

        std::string byr = passport.find("byr")->second;
        std::string iyr = passport.find("iyr")->second;
        std::string eyr = passport.find("eyr")->second;
        std::string hgt = passport.find("hgt")->second;
        std::string hcl = passport.find("hcl")->second;
        std::string ecl = passport.find("ecl")->second;
        std::string pid = passport.find("pid")->second;

        if (!check_year(byr, 1920, 2002))
            continue;

        if (!check_year(iyr, 2010, 2020))
            continue;

        if (!check_year(eyr, 2020, 2030))
            continue;

        // check hgt
        std::istringstream is{hgt};
        int num = 0;
        std::string unit;
        is >> num >> unit;
        if (!((unit.compare("cm") == 0 && num >= 150 && num <= 193)
              || (unit.compare("in") == 0 && num >= 59 && num <= 76)))
            continue;

        // check hcl
        std::regex hcl_regex{"#[0-9a-f]{6}"};
        if (!std::regex_match(hcl, hcl_regex))
            continue;
                
        // check ecl
        if (ecl.compare("amb") != 0
            && ecl.compare("blu") != 0
            && ecl.compare("brn") != 0
            && ecl.compare("gry") != 0
            && ecl.compare("grn") != 0
            && ecl.compare("hzl") != 0
            && ecl.compare("oth") != 0)
            continue;

        // check pid
        std::regex pid_regex{"[0-9]{9}"};
        if (!std::regex_match(pid, pid_regex))
            continue;

        valid_num++;
    }

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

    int valid_num;

    valid_num = solution_for_puzzle_1(passports);
    std::cout << "The number of valid passports: " << valid_num << " (182 expected)\n";

    valid_num = solution_for_puzzle_2(passports);
    std::cout << "The number of strictly valid passports: " << valid_num << " (109 expected)\n";

    return 0;
}
