#include <string>
#include <iostream>
#include <cassert>

// we keep the current cup to be the first one
static std::string one_move(std::string circle)
{
    std::string three_cups = circle.substr(1, 3);
    // remove the three cups
    circle.erase(1, 3);

    char current_cup = circle[0];
    char dest_cup = current_cup;
    do {
        dest_cup--;
        if (dest_cup < '1')
            dest_cup = '9';
    } while (three_cups.find(dest_cup) != std::string::npos);

    size_t dest = circle.find(dest_cup);

    std::string result = circle.substr(0, dest + 1) + three_cups + circle.substr(dest + 1);

    // move the next current cup to the first place
    result = result.substr(1) + result[0];

    return result;
}

static std::string solution_for_puzzle_1(std::string circle)
{
    for (int i = 0; i < 100; i++)
        circle = one_move(circle);

    size_t idx = circle.find('1');
    assert(idx != std::string::npos);

    std::string result = circle.substr(idx + 1) + circle.substr(0, idx);

    return result;
}

int main()
{
    const std::string input{"624397158"};
    // below is the test in the puzzle
    //const std::string input{"389125467"};

    std::string result = solution_for_puzzle_1(input);
    std::cout << "result is " << result << "\n";

    return 0;
}
