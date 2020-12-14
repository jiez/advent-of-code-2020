#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cstdlib>


static uint64_t solution_for_puzzle_1(const std::vector<std::string>& input)
{
    uint64_t mask;
    uint64_t mask_mask;
    std::map<uint64_t, uint64_t> memory;

    for (auto line: input) {
        if (line.substr(0, 3) == "mas") {
            std::string mask_str = line.substr(7, 36);
            mask_mask = 0;
            mask = 0;
            for (int i = 0; i < 36; i++) {
                if (mask_str[i] == 'X')
                    mask_mask |= ((uint64_t) 1) << (36 - i - 1);
                else { // (mask_str[i] == '0' || mask_str[i] == '1')
                    uint64_t bit = mask_str[i] - '0';
                    mask |= bit << (36 - i - 1);
                }
            }
        } else { // mem
            std::regex mem_regex{"mem\\[([0-9]+)\\] = ([0-9]+)"};
            std::smatch m;
            std::regex_search(line, m, mem_regex);
            std::string addr_str = m[1];
            std::string val_str = m[2];
            uint64_t addr = stoull(addr_str);
            uint64_t val = stoull(val_str);

            val &= mask_mask;
            val |= mask;

            memory[addr] = val;
        }
    }

    uint64_t sum = 0;

    for (auto mem: memory) {
        sum += mem.second;
    }

    return sum;
}


int main()
{
    std::vector<std::string> input;
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        input.push_back(line);
    }

    uint64_t sum;

    sum = solution_for_puzzle_1(input);
    std::cout << "sum is " << sum << "\n";
    assert(sum == 13105044880745);

    return 0;
}
