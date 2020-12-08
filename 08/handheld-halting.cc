#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct insn_t {
    std::string name;
    int arg;
};

int solution_for_puzzle_1(std::vector<insn_t> &program)
{
    int pc = 0;
    int acc = 0;
    std::vector<bool> executed(program.size(), false);

    while (1) {
        insn_t insn = program[pc];

        if (executed[pc])
            return acc;
        else
            executed[pc] = true;

        if (insn.name.compare("nop") == 0) {
            pc++;
        } else if (insn.name.compare("acc") == 0) {
            acc += insn.arg;
            pc++;
        } else if (insn.name.compare("jmp") == 0) {
            pc += insn.arg;
        }
    }

    return 0;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    std::vector<insn_t> program;

    while (getline(input_file, line)) {
        //std::cout << line << "\n";
        std::string insn_name;
        int arg;
        std::istringstream is{line};
        is >> insn_name >> arg;
        //std::cout << insn_name << "," << arg << "\n";
        insn_t insn{insn_name, arg};
        program.push_back(insn);
    }

    int result;

    result = solution_for_puzzle_1(program);
    std::cout << "acc value before first repeat: " << result << "\n";

    return 0;
}
