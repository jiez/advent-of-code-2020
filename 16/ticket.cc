#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cstdlib>


struct range_t {
    int min;
    int max;
};

static bool field_valid(const std::vector<std::vector<range_t>>& valid_ranges, int field)
{
    for (auto ranges: valid_ranges)
        for (auto range: ranges)
            if (field >= range.min && field <= range.max)
                return true;

    return false;
}

static int solution_for_puzzle_1(const std::vector<std::vector<range_t>>& valid_ranges, const std::vector<std::vector<int>>& nearby_tickets)
{
    int error_rate = 0;

    for (auto ticket: nearby_tickets)
        for (auto field: ticket)
            if (!field_valid(valid_ranges, field))
                error_rate += field;

    return error_rate;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<std::vector<range_t>> valid_ranges;
    std::vector<int> my_ticket;
    std::vector<std::vector<int>> nearby_tickets;

    std::regex valid_regex{"[a-z]*: ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)"};

    std::string line;
    while (getline(input_file, line)) {
        if (line == "")
            continue;
        if (line == "your ticket:")
            break;

        std::vector<range_t> ranges;

        std::smatch m;
        std::regex_search(line, m, valid_regex);
        int min1 = stoi(m[1]);        
        int max1 = stoi(m[2]);        
        int min2 = stoi(m[3]);        
        int max2 = stoi(m[4]);        

        range_t r1, r2;
        r1.min = min1;
        r1.max = max1;
        r2.min = min2;
        r2.max = max2;

        ranges.push_back(r1);
        ranges.push_back(r2);

        valid_ranges.push_back(ranges);
    }

    {
        getline(input_file, line);
        std::istringstream is(line);
        std::string field;
        while (getline(is, field, ','))
            my_ticket.push_back(stoi(field));
    }


    while (getline(input_file, line))
        if (line == "nearby tickets:")
            break;

    while (getline(input_file, line)) {
        std::istringstream is(line);
        std::string field;
        std::vector<int> ticket;
        while (getline(is, field, ','))
            ticket.push_back(stoi(field));

        nearby_tickets.push_back(ticket);
    }

    int error_rate;

    error_rate = solution_for_puzzle_1(valid_ranges, nearby_tickets);
    std::cout << "error rate is " << error_rate << "\n";

    return 0;
}
