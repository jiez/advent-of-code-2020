#include <cassert>
#include <string>
#include <iostream>
#include <algorithm>
#include <deque>
#include <iterator>

// we keep the current cup to be the first one
static void one_move(std::deque<int>& circle)
{
    auto s = circle.begin() + 1;
    auto t = circle.begin() + 4;
    std::deque<int> three_cups(s, t);

    // remove the three cups
    circle.erase(s, t);

    int current_cup = circle[0];
    int dest_cup = current_cup;
    do {
        dest_cup--;
        if (dest_cup < 1)
            dest_cup = 9;
    } while (std::find(three_cups.begin(), three_cups.end(), dest_cup) != three_cups.end());

    auto dest = std::find(circle.begin(), circle.end(), dest_cup);

    circle.insert(dest + 1, three_cups.begin(), three_cups.end());

    // move the next current cup to the first place
    int first = circle.front();
    circle.pop_front();
    circle.push_back(first);
}

static void solution_for_puzzle_1(std::deque<int>& circle)
{
    for (int i = 0; i < 100; i++)
        one_move(circle);
}

int main()
{
    const std::string input{"624397158"};
    // below is the test in the puzzle
    //const std::string input{"389125467"};

    std::deque<int> circle;
    for (auto c: input)
        circle.push_back(c - '0');

    solution_for_puzzle_1(circle);

    auto idx = std::find(circle.begin(), circle.end(), 1);
    std::cout << "result is ";
    for (auto it = std::next(idx); it != circle.end(); ++it)
        std::cout << *it;
    for (auto it = circle.begin(); it != idx; ++it)
        std::cout << *it;
    std::cout << "\n";

    return 0;
}
