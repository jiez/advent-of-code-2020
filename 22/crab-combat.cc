#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <set>


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


static unsigned long long recurse(std::queue<int> cards1, std::queue<int> cards2)
{
    bool player1_win;

    std::set<std::pair<std::queue<int>, std::queue<int>>> past;

    while (cards1.size() > 0 && cards2.size() > 0) {
        // if cards1 and cards2 seen before, player1 win
        std::pair<std::queue<int>, std::queue<int>> p{cards1, cards2};
        if (past.find(p) != past.end()) {
            player1_win = true;
            break;
        } else
            past.insert(p);

        if (cards1.front() <= cards1.size() - 1 && cards2.front() <= cards2.size() - 1) {
            std::queue<int> cards1_sub = cards1;
            std::queue<int> cards2_sub = cards2;
            int num1 = cards1_sub.front();
            int num2 = cards2_sub.front();
            cards1_sub.pop();
            cards2_sub.pop();

            for (int i = 0; i < num1; i++) {
                cards1_sub.push(cards1_sub.front());
                cards1_sub.pop();
            }
            for (int i = 0; i < cards1_sub.size() - num1; i++)
                cards1_sub.pop();

            for (int i = 0; i < num2; i++) {
                cards2_sub.push(cards2_sub.front());
                cards2_sub.pop();
            }
            for (int i = 0; i < cards2_sub.size() - num2; i++)
                cards2_sub.pop();

            player1_win = recurse(cards1_sub, cards2_sub);
        } else
            player1_win = (cards1.front() > cards2.front());

        if (player1_win) {
            cards1.push(cards1.front());
            cards1.push(cards2.front());
        } else {
            cards2.push(cards2.front());
            cards2.push(cards1.front());
        }
        cards1.pop();
        cards2.pop();
    }

    return player1_win;
}

static unsigned long long solution_for_puzzle_2(std::queue<int> cards1, std::queue<int> cards2)
{
    bool player1_win;

    std::set<std::pair<std::queue<int>, std::queue<int>>> past;

    while (cards1.size() > 0 && cards2.size() > 0) {
        // if cards1 and cards2 seen before, player1 win
        std::pair<std::queue<int>, std::queue<int>> p{cards1, cards2};
        if (past.find(p) != past.end()) {
            player1_win = true;
            break;
        } else
            past.insert(p);

        if (cards1.front() <= cards1.size() - 1 && cards2.front() <= cards2.size() - 1) {
            std::queue<int> cards1_sub = cards1;
            std::queue<int> cards2_sub = cards2;
            int num1 = cards1_sub.front();
            int num2 = cards2_sub.front();
            cards1_sub.pop();
            cards2_sub.pop();

            for (int i = 0; i < num1; i++) {
                cards1_sub.push(cards1_sub.front());
                cards1_sub.pop();
            }
            for (int i = 0; i < cards1_sub.size() - num1; i++)
                cards1_sub.pop();

            for (int i = 0; i < num2; i++) {
                cards2_sub.push(cards2_sub.front());
                cards2_sub.pop();
            }
            for (int i = 0; i < cards2_sub.size() - num2; i++)
                cards2_sub.pop();

            player1_win = recurse(cards1_sub, cards2_sub);
        } else
            player1_win = (cards1.front() > cards2.front());

        if (player1_win) {
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
    if (player1_win)
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

    unsigned long long score;

    score = solution_for_puzzle_1(cards1, cards2);
    std::cout << "score is " << score << "\n";

    score = solution_for_puzzle_2(cards1, cards2);
    std::cout << "recursive score is " << score << "\n";

    return 0;
}
