#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


static void print_seats(const std::vector<std::string>& seats)
{
    for (auto str: seats)
        std::cout << str << "\n";
    std::cout << "\n";
}

static int count_adjacent_occupied1(std::vector<std::string>& seats, int row, int col)
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

static int count_adjacent_occupied2(std::vector<std::string>& seats, int row, int col)
{
    int rows = seats.size();
    int cols = seats[0].size();
    int n = 0;
    int i, j;

    i = row - 1; j = col - 1;
    while (i >= 0 && j >= 0) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i--; j--;
    }

    i = row - 1; j = col;
    while (i >= 0) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i--;
    }

    i = row - 1; j = col + 1;
    while (i >= 0 && j < cols) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i--; j++;
    }

    i = row; j = col - 1;
    while (j >= 0) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        j--;
    }

    i = row; j = col + 1;
    while (j < cols) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        j++;
    }

    i = row + 1; j = col - 1;
    while (i < rows && j >= 0) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i++; j--;
    }

    i = row + 1; j = col;
    while (i < rows) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i++;
    }

    i = row + 1; j = col + 1;
    while (i < rows && j < cols) {
        if (seats[i][j] == '#') {
            n++;
            break;
        } else if (seats[i][j] == 'L')
            break;
        i++; j++;
    }

    return n;
}

typedef int (*count_occupied_func_t)(std::vector<std::string>&, int /*row*/, int /*col*/);

static int solution_common(std::vector<std::string>& seats, count_occupied_func_t f, int tolerance)
{
    std::vector<std::string> seats2 = seats;
    bool changed;

    int rows = seats.size();
    int cols = seats[0].size();

    do { 
        changed = false;
        //print_seats(seats);

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                int n = f(seats, i, j);
                if (seats[i][j] == 'L' && n == 0) {
                    seats2[i][j] = '#';
                    changed = true;
                } else if (seats[i][j] == '#' && n >= tolerance) {
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

static int solution_for_puzzle_1(std::vector<std::string> seats)
{
    return solution_common(seats, count_adjacent_occupied1, 4);
}

static int solution_for_puzzle_2(std::vector<std::string> seats)
{
    return solution_common(seats, count_adjacent_occupied2, 5);
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

    int occupied;

    occupied = solution_for_puzzle_1(seats);
    std::cout << occupied << " seats is occupied\n";
    assert(occupied == 2489);

    occupied = solution_for_puzzle_2(seats);
    std::cout << occupied << " seats is occupied\n";
    assert(occupied == 2180);

    return 0;
}
