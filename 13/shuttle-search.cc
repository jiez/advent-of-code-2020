#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>


static int solution_for_puzzle_1(const std::vector<int>& ids, int earliest_time)
{
    int earliest_id;
    int shortest_wait_time = -1;
    for (auto id: ids) {
        int wait_time = (earliest_time / id + 1) * id - earliest_time;
        if (shortest_wait_time == -1 || wait_time < shortest_wait_time) {
            shortest_wait_time = wait_time;
            earliest_id = id;
        }
    }

    return earliest_id * shortest_wait_time;
}

static unsigned long long solution_for_puzzle_2(const std::vector<int>& ids, const std::vector<int>& positions)
{
    int max_id = 0;
    int max_id_pos;
    int max_id_idx;

    for (int i = 0; i < ids.size(); i++)
        if (ids[i] > max_id) {
            max_id = ids[i];
            max_id_pos = positions[i];
            max_id_idx = i;
        }
    for (unsigned long long t = max_id; true; t += max_id) {
        bool found = true;

        for (int i = 0; i < ids.size(); i++) {
            if (i == max_id_idx)
                continue;

            if ((t - max_id_pos + positions[i]) % ids[i] != 0) {
                found = false;
                break;
            }
        }

        if (found)
            return t - max_id_pos;
    }
}


int main()
{
    int earliest_time;
    std::vector<int> ids;
    std::vector<int> positions;
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    getline(input_file, line);
    //std::cout << line << "\n";
    std::istringstream is{line};        
    is >> earliest_time;
    //std::cout << earliest_time << "\n";
    
    getline(input_file, line);
    //std::cout << line << "\n";
    std::istringstream is2{line};        
    std::string id_str;
    int position = 0;
    while (getline(is2, id_str, ',')) {
        if (id_str == "x") {
            position++;
            continue;
        }
        std::istringstream is3{id_str};        
        int id;
        is3 >> id;
        ids.push_back(id);
        positions.push_back(position);
        //std::cout << position << ":" << id << "\n";
        position++;
    }

    int result = solution_for_puzzle_1(ids, earliest_time);
    std::cout << "result is " << result << "\n";

    unsigned long long result2 = solution_for_puzzle_2(ids, positions);
    std::cout << "result is " << result2 << "\n";
    return 0;
}
