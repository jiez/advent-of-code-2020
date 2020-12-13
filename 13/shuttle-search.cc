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


int main()
{
    int earliest_time;
    std::vector<int> ids;
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
    while (getline(is2, id_str, ',')) {
        if (id_str == "x")
            continue;
        std::istringstream is3{id_str};        
        int id;
        is3 >> id;
        ids.push_back(id);
        //std::cout << id << "\n";
    }

    int result = solution_for_puzzle_1(ids, earliest_time);
    std::cout << "result is " << result << "\n";

    return 0;
}
