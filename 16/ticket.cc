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

static bool field_valid_for_ranges(const std::vector<range_t>& ranges, int field)
{
    for (auto range: ranges)
        if (field >= range.min && field <= range.max)
            return true;

    return false;
}

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

static unsigned long long solution_for_puzzle_2(
    const std::vector<std::string>& field_names,
    const std::vector<std::vector<range_t>>& valid_ranges,
    const std::vector<int>& my_ticket,
    const std::vector<std::vector<int>>& nearby_tickets)
{
    std::vector<std::vector<int>> valid_tickets;
    std::map<int, int> field_map;

    for (auto ticket: nearby_tickets) {
        bool is_valid = true;
        for (auto field: ticket)
            if (!field_valid(valid_ranges, field)) {
                is_valid = false;
                break;
            }

        if (is_valid)
            valid_tickets.push_back(ticket);
    }

    // possible fields in tickets for each field name
    std::vector<std::vector<int>> possible_fields;
    int num_fields = valid_ranges.size();

    for (int i = 0; i < num_fields; i++) {
        std::vector<range_t> ranges = valid_ranges[i];
        std::vector<int> fields;

        for (int j = 0; j < num_fields; j++) {
            bool is_valid = true;
            for (auto ticket: valid_tickets) {
                if (!field_valid_for_ranges(ranges, ticket[j])) {
                    is_valid = false;
                    break;
                }
            }

            if (is_valid)
                fields.push_back(j);
        }

        assert(fields.size() > 0);

        possible_fields.push_back(fields);
    }

    /*
    for (int i = 0; i < num_fields; i++) {
        std::cout << i << ":";
        for (auto field: possible_fields[i])
            std::cout << " " << field;
        std::cout << "\n";
    }
    */

    while (true) {
        int fixed;
        int i;
        for (i = 0; i < possible_fields.size(); i++)
            if (possible_fields[i].size() == 1) {
                fixed = possible_fields[i][0];
                field_map[i] = fixed;
                break;
            }

        if (i == possible_fields.size())
            break;

        for (i = 0; i < possible_fields.size(); i++) {
            auto found = std::find(possible_fields[i].begin(), possible_fields[i].end(), fixed);
            if (found != possible_fields[i].end())
                possible_fields[i].erase(found);
        }
    }

    /*
    for (int i = 0; i < num_fields; i++)
        std::cout << i << ":" << field_map[i] << "\n";
    */

    unsigned long long result = 1;
    for (int i = 0; i < num_fields; i++)
        if (field_names[i].substr(0, 9) == "departure")
            result *= my_ticket[field_map[i]];
        
    return result;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<std::string> field_names;
    std::vector<std::vector<range_t>> valid_ranges;
    std::vector<int> my_ticket;
    std::vector<std::vector<int>> nearby_tickets;

    std::regex valid_regex{"([a-z ]*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)"};

    std::string line;
    while (getline(input_file, line)) {
        if (line == "")
            continue;
        if (line == "your ticket:")
            break;

        std::vector<range_t> ranges;

        std::smatch m;
        std::regex_search(line, m, valid_regex);
        std::string field_name = m[1];
        int min1 = stoi(m[2]);        
        int max1 = stoi(m[3]);        
        int min2 = stoi(m[4]);        
        int max2 = stoi(m[5]);        

        range_t r1, r2;
        r1.min = min1;
        r1.max = max1;
        r2.min = min2;
        r2.max = max2;

        ranges.push_back(r1);
        ranges.push_back(r2);

        valid_ranges.push_back(ranges);

        field_names.push_back(field_name);
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

    unsigned long long result = solution_for_puzzle_2(field_names, valid_ranges, my_ticket, nearby_tickets);
    std::cout << "result is " << result << "\n";
    return 0;
}
