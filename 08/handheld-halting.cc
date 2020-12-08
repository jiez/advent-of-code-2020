#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct insn_t {
    std::string name;
    int arg;
};

static bool execute(std::vector<insn_t>& program, int* ret_acc)
{
    int pc = 0;
    int acc = 0;
    std::vector<bool> executed(program.size(), false);

    while (1) {
        insn_t insn = program[pc];

        if (pc == program.size())
            break;

        if (executed[pc]) {
            *ret_acc = acc;
            return false;
        }

        executed[pc] = true;

        if (insn.name == "nop") {
            pc++;
        } else if (insn.name == "acc") {
            acc += insn.arg;
            pc++;
        } else if (insn.name == "jmp") {
            pc += insn.arg;
        }
    }

    *ret_acc = acc;
    return true;
}


int solution_for_puzzle_1(std::vector<insn_t> &program)
{
    int acc;
    bool halted = execute(program, &acc);

    assert(! halted);

    return acc;
}

int solution_for_puzzle_2(std::vector<insn_t> &program)
{
    for (int i = 0; i < program.size(); i++) {
        std::string orig_name = program[i].name;

        if (program[i].name == "nop") {
            program[i].name = "jmp";
        } else if (program[i].name == "jmp") {
            program[i].name = "nop";
        } else
            continue;

        int acc;
        bool halted = execute(program, &acc);

        if (halted)
            return acc;

        program[i].name = orig_name;
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
    std::cout << "acc value before first repeat: " << result << " (1782 expected)\n";

    result = solution_for_puzzle_2(program);
    std::cout << "acc value when halt: " << result << " (797 expected)\n";

    return 0;
}
