#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <array>

static int num_players_have_cards(const std::array<std::queue<int>, 2>& cards)
{
    int count = 0;
    for (auto player: cards)
        if (player.size() > 0)
            count++;

    return count;
}

static unsigned long long solution_for_puzzle_1(std::array<std::queue<int>, 2>& cards)
{
    while(num_players_have_cards(cards) > 1) {
        if (cards[0].front() > cards[1].front()) {
            cards[0].push(cards[0].front());
            cards[0].push(cards[1].front());
        } else {
            cards[1].push(cards[1].front());
            cards[1].push(cards[0].front());
        }
        cards[0].pop();
        cards[1].pop();
    }

    unsigned long long score = 0;
    for (auto player: cards)
        if (player.size() > 0) {
            int mult = player.size();
            while (!player.empty()) {
                score += player.front() * mult;
                player.pop();
                mult--;
            }
            return score;
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

    std::array<std::queue<int>, 2> cards;

    std::string line;
    int player;
    while (getline(input_file, line)) {
        if (line == "")
            continue;

        if (line.compare(0, 6, "Player") == 0) {
            player = stoi(line.substr(7));
            continue;
        }

        cards[player - 1].push(stoi(line));            
    }

    unsigned long long score = solution_for_puzzle_1(cards);
    std::cout << "score is " << score << "\n";

    return 0;
}
