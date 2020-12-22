#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <set>

//#define DEBUG

static int last_game;

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

static void print_cards(std::queue<int> cards)
{
    while (!cards.empty()) {
        std::cout << cards.front();
        cards.pop();
        if (!cards.empty())
            std::cout << ", ";
    }
}

// return 1 if player 1 win, otherwise return 0
// but for game 0, return the score of winner instead
static unsigned long long recurse(std::queue<int> cards1, std::queue<int> cards2, int from_game)
{
    bool player1_win;

    last_game++;
    int game = last_game;
    int round = 1;

    std::set<std::pair<std::queue<int>, std::queue<int>>> past;

#ifdef DEBUG
    std::cout << "=== Game " << game << " ===\n";
#endif

    while (cards1.size() > 0 && cards2.size() > 0) {
#ifdef DEBUG
        std::cout << "\n-- Round " << round << " (Game " << game << ") --\n";
        std::cout << "Player 1's deck: "; print_cards(cards1); std::cout << "\n";
        std::cout << "Player 2's deck: "; print_cards(cards2); std::cout << "\n";
#endif

        // if cards1 and cards2 seen before, player1 win
        std::pair<std::queue<int>, std::queue<int>> p{cards1, cards2};
        if (past.find(p) != past.end()) {
#ifdef DEBUG
            std::cout << "same cards as a previous round\n";
            std::cout << "Player 1 wins round " << round << " of game " << game << "!\n";
#endif
            player1_win = true;
            break;
        } else
            past.insert(p);

#ifdef DEBUG
        std::cout << "Player 1 plays: " << cards1.front() << "\n";
        std::cout << "Player 2 plays: " << cards2.front() << "\n";
#endif

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
            int pop_num1 = cards1_sub.size() - num1;
            for (int i = 0; i < pop_num1; i++)
                cards1_sub.pop();

            for (int i = 0; i < num2; i++) {
                cards2_sub.push(cards2_sub.front());
                cards2_sub.pop();
            }
            int pop_num2 = cards2_sub.size() - num2;
            for (int i = 0; i < pop_num2; i++)
                cards2_sub.pop();

#ifdef DEBUG
            std::cout << "Playing a sub-game to determine the winner...\n\n";
#endif
            player1_win = recurse(cards1_sub, cards2_sub, game);
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

#ifdef DEBUG
        std::cout << "Player " << (player1_win ? 1 : 2) << " wins round " << round << " of game " << game << "!\n";
#endif
        round++;
    }

#ifdef DEBUG
    std::cout << "The winner of game " << game << " is player " << (player1_win ? 1 : 2) << "!\n\n";

    if (game == 1) {
        std::cout << "\n== Post-game results ==\n";
        std::cout << "Player 1's deck: "; print_cards(cards1); std::cout << "\n";
        std::cout << "Player 2's deck: "; print_cards(cards2); std::cout << "\n";
    } else
        std::cout << "...anyway, back to game " << from_game << ".\n";
#endif

    if (game != 1)
        return player1_win;

    // for game 1, return score of winner
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

static unsigned long long solution_for_puzzle_2(std::queue<int> cards1, std::queue<int> cards2)
{
    last_game = 0;

    return recurse(cards1, cards2, 0);
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
