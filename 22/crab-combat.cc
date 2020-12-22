#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>


static unsigned long long solution_for_puzzle_1(std::queue<int> cards1, std::queue<int> cards2)
{
    while(cards1.size() > 0 && cards2.size() > 0) {
        if (cards1.front() > cards2.front()) {
            cards1.push(cards1.front());
            cards1.push(cards2.front());
        } else {
            cards2.push(cards2.front());
            cards2.push(cards1.front());
        }
        cards1.pop();
        cards2.pop();
    }

    unsigned long long score = 0;
    std::queue<int> winner;
    if (cards1.size() > 0)
        winner = cards1;
    else
        winner = cards2;

    int mult = winner.size();
    while (!winner.empty()) {
        score += winner.front() * mult;
        winner.pop();
        mult--;
    }
    return score;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::queue<int> cards1, cards2;
    std::queue<int>* cards;

    std::string line;
    int player;
    while (getline(input_file, line)) {
        if (line == "")
            continue;

        if (line.compare(0, 6, "Player") == 0) {
            player = stoi(line.substr(7));
            if (player == 1)
                cards = &cards1;
            else
                cards = &cards2;
            continue;
        }

        cards->push(stoi(line));            
    }

    unsigned long long score = solution_for_puzzle_1(cards1, cards2);
    std::cout << "score is " << score << "\n";

    return 0;
}
