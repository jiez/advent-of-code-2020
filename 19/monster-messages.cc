#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

struct nfa_t {
    int start_state;
    int end_state;
    int last_state;
    std::set<int> alphabet;
    std::multimap<std::pair<int, int>, int> transitions;

    nfa_t(const std::map<int, std::vector<std::vector<int>>>& rules, int rule0_no) {
        // from rules we create an NFA

        start_state = 0;
        end_state = 1;
        last_state = 1;

        // create the first transition and put it in working set
        //               rule 0
        // start_state ----------> end_state
        std::multimap<std::pair<int, int>, int> working;
        std::pair<int, int> move_from{start_state, rule0_no};
        working.insert(std::pair<std::pair<int, int>, int>{move_from, end_state});

        while (!working.empty()) {
            auto it = working.begin();
            int from_state = it->first.first;
            int rule_no = it->first.second;
            int to_state = it->second;

            auto ir = rules.find(rule_no);
            auto subrules = ir->second;

            for (auto subrule: subrules) {
                int fs = from_state;
                for (int i = 0; i < subrule.size(); i++) {
                    std::pair<int, int> mf{fs, subrule[i]};
                    if (i == subrule.size() - 1)
                        fs = to_state;
                    else
                        fs = get_new_state();

                    std::pair<std::pair<int, int>, int> t{mf, fs};

                    if (subrule[i] < 1000) {
                        alphabet.insert(subrule[i]);
                        transitions.insert(t);
                    } else
                        working.insert(t);
                }
            }

            working.erase(it);
        }

        //print();
    }

    void print() {
        std::cout << "NFA\n";
        std::cout << "start: " << start_state << "\n";
        std::cout << "  end: " << end_state << "\n";

        for (auto transition: transitions)
            std::cout << transition.first.first << " --" << (char)(transition.first.second) << "--> " << transition.second << "\n";

        std::cout << "\n";
    }

    int get_new_state() {
        last_state++;
        return last_state;
    }

    std::set<int> transit(const std::set<int>& states, int c) const {
        std::set<int> to;

        for (auto state: states) {
            std::pair<int, int> from{state, c};
            auto range = transitions.equal_range(from);
            for (auto it = range.first; it != range.second; ++it)
                to.insert(it->second);
        }

        return to;
    }
};

struct dfa_t {
    int start_state;
    std::set<int> end_states;
    int next_state;
    std::map<std::pair<int, int>, int> transitions;

    dfa_t(const nfa_t& nfa) {
        next_state = 0;

        // convert NFA to DFA
        // NOTE we don't have \epsilon label

        std::set<int> alphabet = nfa.alphabet;
        std::set<int> ss{nfa.start_state};
        std::set<std::set<int>> states;
        std::set<std::set<int>> working{ss};
        std::map<std::pair<std::set<int>, int>, std::set<int>> trans;

        while (!working.empty()) {
            auto it = working.begin();

            for (auto c: alphabet) {
                std::set<int> to = nfa.transit(*it, c);
                if (to.empty())
                    continue;
                std::pair<std::set<int>, int> from{*it, c};
                std::pair<std::pair<std::set<int>, int>, std::set<int>> t{from, to};
                trans.insert(t);
                if (states.find(to) == states.end()) {
                    states.insert(to);
                    working.insert(to);
                }
            }

            working.erase(it);
        }

        std::map<std::set<int>, int> set2int;
        for (auto state: states) {
            std::pair<std::set<int>, int> s2i{state, get_new_state()};
            set2int.insert(s2i);
        }

        for (auto t: trans) {
            auto from = t.first;
            auto to = t.second;
            std::pair<int, int> from2{set2int[from.first], from.second};
            int to2 = set2int[to];
            std::pair<std::pair<int, int>, int> t2{from2, to2};
            transitions.insert(t2);
        }

        start_state = set2int[ss];
        for (auto state: states)
            if (state.find(nfa.end_state) != state.end())
                end_states.insert(set2int[state]);

        //print();
    }

    void print() {
        std::cout << "DFA\n";
        std::cout << "start: " << start_state << "\n";
        std::cout << "  end:";
        for (auto e: end_states)
            std::cout << " " << e;
        std::cout << "\n";

        for (auto transition: transitions)
            std::cout << transition.first.first << " --" << (char)(transition.first.second) << "--> " << transition.second << "\n";

        std::cout << "\n";
    }

    int get_new_state() {
        int temp = next_state;
        next_state++;
        return temp;
    }

    bool validate(std::string message) {
        int state = start_state;

        for (int i = 0; i < message.size(); i++) {
            std::pair<int, int> move_from{state, message[i]};
            auto transition = transitions.find(move_from);
            if (transition == transitions.end())
                return false;

            state = transition->second;
        }

        return end_states.find(state) != end_states.end();
    }
};

static unsigned long long solution_for_puzzle_1(const std::map<int, std::vector<std::vector<int>>>& rules, int rule0_no, const std::vector<std::string>& messages)
{
    int valid_messages = 0;

    nfa_t nfa(rules, rule0_no);
    dfa_t dfa(nfa);

    for (auto message: messages)
        if (dfa.validate(message))
            valid_messages++;

    return valid_messages;
}

static void print_rule(int rule_no, const std::vector<std::vector<int>>& subrules)
{
    std::cout << rule_no - 1000 << ":";
    for (int i = 0; i < subrules.size(); i++) {
        for (auto r: subrules[i])
            if (r >= 1000)
                std::cout << " " << r - 1000;
            else
                std::cout << " \"" << (char)r << "\"";
        if (i != subrules.size() - 1)
            std::cout << " |";
    }
    std::cout << "\n";
}

int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::map<int, std::vector<std::vector<int>>> rules;
    std::vector<std::string> messages;

    std::string line;
    while (getline(input_file, line)) {
        if (line == "")
            continue;

        // if this is a message
        if (line[0] < '0' || line[0] > '9') {
            messages.push_back(line);
            continue;
        }

        // if this is a rule

        size_t i = 0, next;
        int rule_no;

        rule_no = stoi(line, &next);
        // add 1000 to rule numbers so we can distinguish them with
        // letters
        rule_no += 1000;

        i = next;

        assert(line[i] == ':');
        // skip ':'
        i++;

        std::vector<std::vector<int>> subrules;
        std::vector<int> subrule;
        while (i < line.size()) {
            if (line[i] == ' ') {
                i++;
            }
            else if (line[i] >= '0' && line[i] <= '9') {
                int r = stoi(line.substr(i), &next);
                i += next;
                subrule.push_back(r + 1000);
            }
            else if (line[i] == '"') {
                subrule.push_back(line[i + 1]);
                i += 3;
            }
            else if (line[i] == '|') {
                subrules.push_back(std::move(subrule));
                i++;
            }
        }
        subrules.push_back(std::move(subrule));
        rules[rule_no] = std::move(subrules);
    }

    /*
    for (auto rule: rules)
        print_rule(rule.first, rule.second);
    for (auto message: messages)
        std::cout << message << "\n";
    */

    int result;

    result = solution_for_puzzle_1(rules, 1000, messages);
    std::cout << "The number of valid messages: " << result << "\n";

    return 0;
}
