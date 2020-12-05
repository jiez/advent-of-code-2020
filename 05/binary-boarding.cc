#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

int calculate_seat_id(std::string& pass)
{
    int row_min = 0, row_max = 127;
    int col_min = 0, col_max = 7;

    for (int i = 0; i < 7; i++)
        if (pass[i] == 'F')
            row_max = row_max - (row_max - row_min + 1) / 2;
        else
            row_min = row_min + (row_max - row_min + 1) / 2;

    for (int i = 7; i < 10; i++)
        if (pass[i] == 'L')
            col_max = col_max - (col_max - col_min + 1) / 2;
        else
            col_min = col_min + (col_max - col_min + 1) / 2;

    std::cout << "row: " << row_min << ", " << "col: " << col_min << "\n";

    return row_min * 8 + col_min;
}

int solution_for_puzzle_1(std::vector<std::string> &passes)
{
    int highest_seat_id = 0;

    for (auto pass: passes) {
        int seat_id = calculate_seat_id(pass);
        if (highest_seat_id < seat_id)
            highest_seat_id = seat_id;
    }

    return highest_seat_id;
}

int main()
{
    std::ifstream input_file{"input"};
    std::vector<std::string> passes;

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};
        passes.push_back(line);
    }

    //std::for_each(passes.begin(), passes.end(), [](std::string& line){std::cout << line << "\n";});

    int highest_seat_id = solution_for_puzzle_1(passes);
    std::cout << "The highest seat ID: " << highest_seat_id << "\n";

    return 0;
}
