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

struct memory_t {
    uint64_t val;
    std::vector<std::string> adds;
    std::vector<std::string> subs;
};

static std::string mask_addr(uint64_t addr, const std::string& mask)
{
    std::string new_addr = mask;

    for (int i = 35; i >= 0 && addr > 0; i--) {
        if (mask[i] == '0')
            new_addr[i] = (addr & 0x1) + '0';
        addr >>= 1;
    }

    return new_addr;
}

static std::string addr_intersection(const std::string& addr1,
    const std::string& addr2)
{
    std::string result = addr1;

    for (int i = 0; i < result.size(); i++) {
        if (result[i] == 'X') {
            result[i] = addr2[i];
        } else if (addr2[i] == 'X') {
            // do nothing
        } else if (result[i] != addr2[i])
            return std::string{""};
    }

    return result;
}

static int64_t addr_count(const std::string& addr)
{
    int64_t count = 1;

    for (auto c: addr)
        if (c == 'X')
            count *= 2;

    return count;
}

static uint64_t solution_for_puzzle_2(const std::vector<std::string>& input)
{
    std::vector<memory_t> memory;

    std::string mask;

    for (auto line: input) {
        if (line.substr(0, 3) == "mas") {
            mask = line.substr(7, 36);
        } else { // mem
            std::regex mem_regex{"mem\\[([0-9]+)\\] = ([0-9]+)"};
            std::smatch m;
            std::regex_search(line, m, mem_regex);
            std::string addr_str = m[1];
            std::string val_str = m[2];
            uint64_t addr = stoull(addr_str);
            uint64_t val = stoull(val_str);
            std::string addr36 = mask_addr(addr, mask);

            // update all previous memory
            for (auto& mem: memory) {
                std::vector<std::string> new_adds;
                std::vector<std::string> new_subs;

                for (auto add: mem.adds) {
                    std::string t = addr_intersection(add, addr36);
                    if (t.size() > 0)
                        new_subs.push_back(t);
                }
                for (auto sub: mem.subs) {
                    std::string t = addr_intersection(sub, addr36);
                    if (t.size() > 0)
                        new_adds.push_back(t);
                }

                std::move(new_adds.begin(), new_adds.end(),
                    std::back_inserter(mem.adds));
                std::move(new_subs.begin(), new_subs.end(),
                    std::back_inserter(mem.subs));
            }

            // add this memory
            memory_t mem;
            mem.val = val;
            mem.adds.push_back(addr36);
            memory.push_back(mem);
        }
    }

    uint64_t sum = 0;

    for (auto mem: memory) {
        int64_t count = 0;

        for (auto add: mem.adds)
            count += addr_count(add);
        for (auto sub: mem.subs)
            count -= addr_count(sub);

        assert(count >= 0);

        uint64_t old = sum;
        sum += mem.val * count;

        assert(count == 0 || mem.val * count > mem.val);
        assert(sum >= old);
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

    sum = solution_for_puzzle_2(input);
    std::cout << "sum is " << sum << "\n";
    assert(sum == 3505392154485);
 
    return 0;
}
