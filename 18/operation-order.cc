#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>


static unsigned long long evaluate(const std::string& line, size_t *idx = 0)
{
    size_t i = 0;
    unsigned long long result;
    char op = ' ';

    while (i < line.size()) {
        if (line[i] == ' ') {
            i++;
        }
        else if (line[i] >= '0' && line[i] <= '9') {
            size_t next;
            unsigned long long val = stoull(line.substr(i), &next);

            if (op == '*')
                result *= val;
            else if (op == '+')
                result += val;
            else
                result = val;

            i += next;
        }
        else if (line[i] == '*' || line[i] == '+') {
            op = line[i];
            i++;
        }
        else if (line[i] == '(') {
            size_t next;
            unsigned long long val = evaluate(line.substr(i + 1), &next);

            if (op == '*')
                result *= val;
            else if (op == '+')
                result += val;
            else
                result = val;

            i += next + 1;
        }
        else if (line[i] == ')') {
            *idx = i + 1;
            return result;
        }
    }

    return result;
}

static unsigned long long solution_for_puzzle_1(const std::vector<std::string>& input)
{
    unsigned long long sum = 0;

    for (auto line: input)
        sum += evaluate(line);

    return sum;
}

static unsigned long long evaluate2(const std::string& line, size_t *idx = 0)
{
    std::stack<unsigned long long> vals;
    std::stack<char> ops;
    size_t i = 0;

    while (i < line.size()) {
        if (line[i] == ' ') {
            i++;
        }
        else if (line[i] >= '0' && line[i] <= '9') {
            size_t next;
            unsigned long long val = stoull(line.substr(i), &next);

            if (ops.empty())
                vals.push(val);
            else if (ops.top() == '+') {
                vals.top() += val;
                ops.pop();
            }
            else if (ops.top() == '*')
                vals.push(val);

            i += next;
        }
        else if (line[i] == '*' || line[i] == '+') {
            ops.push(line[i]);
            i++;
        }
        else if (line[i] == '(') {
            size_t next;
            unsigned long long val = evaluate2(line.substr(i + 1), &next);

            if (ops.empty())
                vals.push(val);
            else if (ops.top() == '+') {
                vals.top() += val;
                ops.pop();
            }
            else if (ops.top() == '*')
                vals.push(val);

            i += next + 1;
        }
        else if (line[i] == ')') {
            unsigned long long result = 1;
            while (!vals.empty()) {
                result *= vals.top();
                vals.pop();
            }
            *idx = i + 1;
            return result;
        }
    }

    unsigned long long result = 1;
    while (!vals.empty()) {
        result *= vals.top();
        vals.pop();
    }

    return result;
}

static unsigned long long solution_for_puzzle_2(const std::vector<std::string>& input)
{
    unsigned long long sum = 0;

    for (auto line: input)
        sum += evaluate2(line);

    return sum;
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<std::string> input;

    std::string line;
    while (getline(input_file, line))
        input.push_back(line);

    unsigned long long sum;

    sum = solution_for_puzzle_1(input);
    std::cout << "sum is " << sum << "\n";
    assert(sum == 4696493914530);

    sum = solution_for_puzzle_2(input);
    std::cout << "sum is " << sum << "\n";
    assert(sum == 362880372308125);

    return 0;
}
