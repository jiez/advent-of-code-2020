#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>


struct shuttle_t {
    int id;
    int pos;
};

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

static unsigned long long solution_for_puzzle_2(std::vector<shuttle_t>& shuttles)
{
    std::sort(shuttles.begin(), shuttles.end(), [](shuttle_t s1, shuttle_t s2) {
        return s1.id > s2.id;
    });

    /*
    for (auto s: shuttles) {
        std::cout << s.pos << ":" << s.id << "\n";
    }
    */

    // brutal force the first N
    int n = 4;
    if (n > shuttles.size())
        n = shuttles.size();

    int max_id = shuttles[0].id;
    int max_id_pos = shuttles[0].pos;
    unsigned long long t;

    for (t = max_id; true; t += max_id) {
        bool found = true;

        for (int i = 1; i < n; i++) {
            if ((t - max_id_pos + shuttles[i].pos) % shuttles[i].id != 0) {
                found = false;
                break;
            }
        }

        if (found)
            break;
    }

    t -= max_id_pos;

    if (n == shuttles.size())
        return t;

    // brutal force the others

    unsigned long long inc = 1;
    for (int i = 0; i < n; i++)
        inc *= shuttles[i].id;

    for (; true; t += inc) {
        bool found = true;

        for (int i = n; i < shuttles.size(); i++) {
            if ((t + shuttles[i].pos) % shuttles[i].id != 0) {
                found = false;
                break;
            }
        }

        if (found)
            break;
    }

    return t;
}


int main()
{
    int earliest_time;
    std::vector<int> ids;
    std::vector<shuttle_t> shuttles;
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

        shuttle_t shuttle;
        shuttle.id = id;
        shuttle.pos = position;
        shuttles.push_back(shuttle);
        //std::cout << shuttle.pos << ":" << shuttle.id << "\n";
        position++;
    }

    int result = solution_for_puzzle_1(ids, earliest_time);
    std::cout << "result is " << result << "\n";
    assert(result = 8063);

    unsigned long long result2 = solution_for_puzzle_2(shuttles);
    std::cout << "result is " << result2 << "\n";
    assert(result2 == 775230782877242);
    return 0;
}
