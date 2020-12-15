#include <iostream>
#include <vector>
#include <map>
#include <cassert>

std::vector<int> start_numbers = {0,13,1,16,6,17};
//std::vector<int> start_numbers = {1, 3, 2};
//std::vector<int> start_numbers = {0, 3, 6};

static int solution_for_puzzle_1(const std::vector<int>& start_numbers, int ord)
{
    std::map<int, std::pair<int, int>> numbers;

    for (int i = 0; i < start_numbers.size(); i++)
        numbers[start_numbers[i]] = std::make_pair(0, i + 1);

    int last_number = start_numbers.back();
    int turn = start_numbers.size() + 1;
    do {
        auto search = numbers.find(last_number);
        assert(search != numbers.end());

        if (search->second.first == 0) {
            int temp = 0;
            auto search2 = numbers.find(temp);
            int last_turn = 0;
            if (search2 != numbers.end())
                last_turn = search2->second.second;
            numbers[temp] = std::make_pair(last_turn, turn);
            last_number = temp;
        } else {
            int temp = search->second.second - search->second.first;
            auto search2 = numbers.find(temp);
            int last_turn = 0;
            if (search2 != numbers.end())
                last_turn = search2->second.second;
            numbers[temp] = std::make_pair(last_turn, turn);
            last_number = temp;
        }
        turn++;
    } while (turn <= ord);

    return last_number;
}

int main ()
{
    int result = solution_for_puzzle_1(start_numbers, 2020);
    std::cout << "2020th: " << result << "\n";

    return 0;
}
