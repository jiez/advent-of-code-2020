#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <unordered_set>

//#define DEBUG

static int last_game;

static unsigned long long solution_for_puzzle_1(std::deque<int> cards1, std::deque<int> cards2)
{
    while(cards1.size() > 0 && cards2.size() > 0) {
        if (cards1.front() > cards2.front()) {
            cards1.push_back(cards1.front());
            cards1.push_back(cards2.front());
        } else {
            cards2.push_back(cards2.front());
            cards2.push_back(cards1.front());
        }
        cards1.pop_front();
        cards2.pop_front();
    }

    unsigned long long score = 0;
    std::deque<int> winner;
    if (cards1.size() > 0)
        winner = cards1;
    else
        winner = cards2;

    int mult = winner.size();
    while (!winner.empty()) {
        score += winner.front() * mult;
        winner.pop_front();
        mult--;
    }
    return score;
}

static void print_cards(const std::deque<int>& cards)
{
    for (int i = 0; i < cards.size(); i++) {
        std::cout << cards[i];
        if (i != cards.size() - 1)
            std::cout << ", ";
    }
}

struct cards_hash {
    size_t operator()(const std::pair<std::deque<int>, std::deque<int>>& cards) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (auto c: cards.first)
            seed ^= hasher(c) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        for (auto c: cards.second)
            seed ^= hasher(c) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        return seed;
    }
};

// return 1 if player 1 win, otherwise return 0
// but for game 0, return the score of winner instead
static unsigned long long recurse(std::deque<int> cards1, std::deque<int> cards2, int from_game)
{
    bool player1_win;

    last_game++;
    int game = last_game;
    int round = 1;

    std::unordered_set<std::pair<std::deque<int>, std::deque<int>>, cards_hash> past;

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
        std::pair<std::deque<int>, std::deque<int>> p{cards1, cards2};
        if (past.find(p) != past.end()) {
#ifdef DEBUG
            std::cout << "same cards as a previous round\n";
            std::cout << "Player 1 wins round " << round << " of game " << game << "!\n";
#endif
            player1_win = true;
            break;
        } else
            past.insert(std::move(p));

#ifdef DEBUG
        std::cout << "Player 1 plays: " << cards1.front() << "\n";
        std::cout << "Player 2 plays: " << cards2.front() << "\n";
#endif

        if (cards1.front() <= cards1.size() - 1 && cards2.front() <= cards2.size() - 1) {
            std::deque<int> cards1_sub;
            std::deque<int> cards2_sub;
            int num1 = cards1.front();
            int num2 = cards2.front();

            std::copy(cards1.begin() + 1, cards1.begin() + 1 + num1,
                      std::back_inserter(cards1_sub));
            std::copy(cards2.begin() + 1, cards2.begin() + 1 + num2,
                      std::back_inserter(cards2_sub));

#ifdef DEBUG
            std::cout << "Playing a sub-game to determine the winner...\n\n";
#endif
            player1_win = recurse(cards1_sub, cards2_sub, game);
        } else
            player1_win = (cards1.front() > cards2.front());

        if (player1_win) {
            cards1.push_back(cards1.front());
            cards1.push_back(cards2.front());
        } else {
            cards2.push_back(cards2.front());
            cards2.push_back(cards1.front());
        }
        cards1.pop_front();
        cards2.pop_front();

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
    std::deque<int> winner;
    if (player1_win)
        winner = cards1;
    else
        winner = cards2;

    int mult = winner.size();
    while (!winner.empty()) {
        score += winner.front() * mult;
        winner.pop_front();
        mult--;
    }
    return score;
}

static unsigned long long solution_for_puzzle_2(std::deque<int> cards1, std::deque<int> cards2)
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

    std::deque<int> cards1, cards2;
    std::deque<int>* cards;

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

        cards->push_back(stoi(line));            
    }

    unsigned long long score;

    score = solution_for_puzzle_1(cards1, cards2);
    std::cout << "score is " << score << "\n";
    assert(score == 33631);

    score = solution_for_puzzle_2(cards1, cards2);
    std::cout << "recursive score is " << score << "\n";
    assert(score == 33469);

    std::cout << last_game << " games\n";

    return 0;
}
