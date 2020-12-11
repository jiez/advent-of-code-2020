#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

static int count_adjacent_occupied(std::vector<std::string>& seats, int row, int col)
{
    int rows = seats.size();
    int cols = seats[0].size();
    int n = 0;

    if (row - 1 >= 0 && col - 1 >= 0 && seats[row - 1][col - 1] == '#')
        n++;

    if (row - 1 >= 0 && seats[row - 1][col] == '#')
        n++;

    if (row - 1 >= 0 && col + 1 < cols && seats[row - 1][col + 1] == '#')
        n++;

    if (col - 1 >= 0 && seats[row][col - 1] == '#')
        n++;

    if (col + 1 < cols && seats[row][col + 1] == '#')
        n++;

    if (row + 1 < rows && col - 1 >= 0 && seats[row + 1][col - 1] == '#')
        n++;

    if (row + 1 < rows && seats[row + 1][col] == '#')
        n++;

    if (row + 1 < rows && col + 1 < cols && seats[row + 1][col + 1] == '#')
        n++;

    return n;
}

static void print_seats(const std::vector<std::string>& seats)
{
    for (auto str: seats)
        std::cout << str << "\n";
    std::cout << "\n";
}

int solution_for_puzzle_1(std::vector<std::string> seats)
{
    std::vector<std::string> seats2 = seats;
    bool changed;

    int rows = seats.size();
    int cols = seats[0].size();

    do { 
        changed = false;
        print_seats(seats);

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                int n = count_adjacent_occupied(seats, i, j);
                if (seats[i][j] == 'L' && n == 0) {
                    seats2[i][j] = '#';
                    changed = true;
                } else if (seats[i][j] == '#' && n >= 4) {
                    seats2[i][j] = 'L';
                    changed = true;
                } else
                    seats2[i][j] = seats[i][j];
            }

        std::vector<std::string> temp = std::move(seats);
        seats = std::move(seats2);
        seats2 = std::move(temp);
    } while (changed);

    int occupied = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (seats[i][j] == '#')
                occupied++;

    return occupied;
}


int main()
{
    std::vector<std::string> seats;
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        seats.push_back(line);
    }

    int occupied = solution_for_puzzle_1(seats);
    std::cout << occupied << " seats is occupied\n";

    return 0;
}
