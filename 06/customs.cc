#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>


int solution_for_puzzle_1(std::vector<std::string> &customs)
{
    int sum = 0;
    bool questions[26];

    for (int i = 0; i < 26; i++)
        questions[i] = false;

    for (auto custom: customs) {
        if (custom.size() == 0) {
            int n = 0;
            for (auto q: questions)
                if (q)
                    n++;
            sum += n;
            
            for (int i = 0; i < 26; i++)
                questions[i] = false;

            continue;
        }

        for (auto c: custom)
            questions[c - 'a'] = true;
    }

    int n = 0;
    for (auto q: questions)
        if (q)
            n++;
    sum += n;
 
    return sum;
}

int solution_for_puzzle_2(std::vector<std::string> &customs)
{
    int sum = 0;
    int questions[26];

    for (int i = 0; i < 26; i++)
        questions[i] = 0;

    int people = 0;
    for (auto custom: customs) {
        if (custom.size() == 0) {
            int n = 0;
            for (auto q: questions)
                if (q == people)
                    n++;
            sum += n;
            
            for (int i = 0; i < 26; i++)
                questions[i] = 0;

            people = 0;

            continue;
        }

        for (auto c: custom)
            questions[c - 'a']++;

        people++;
    }

    int n = 0;
    for (auto q: questions)
        if (q == people)
            n++;
    sum += n;
 
    return sum;
}


int main()
{
    std::ifstream input_file{"input"};
    std::vector<std::string> customs;

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        customs.push_back(line);
    }

    //std::for_each(passes.begin(), passes.end(), [](std::string& line){std::cout << line << "\n";});

    int result = solution_for_puzzle_1(customs);
    std::cout << "The sum is: " << result << "\n";

    int result2 = solution_for_puzzle_2(customs);
    std::cout << "The sum is: " << result2 << "\n";

    return 0;
}
