#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>


struct action_t {
    char act;
    int val;
};

static int solution_for_puzzle_1(const std::vector<action_t>& actions)
{
    int x = 0, y = 0;
    int dir_x = 1, dir_y = 0;

    for (auto a: actions) {
        int angle;

        switch (a.act) {
        case 'N':
            y += a.val;
            break;

        case 'S':
            y -= a.val;
            break;;

        case 'E':
            x += a.val;
            break;

        case 'W':
            x -= a.val;
            break;

        case 'L':
            if (a.val == 90)
                angle = 270;
            else if (a.val == 180)
                angle = 180;
            else if (a.val == 270)
                angle = 90;
            else
                angle = 0;
            // fall through
        case 'R':
            if (a.act == 'R')
                angle = a.val;

            if (angle == 90) {
                int temp = dir_x;
                dir_x = dir_y;
                dir_y = -temp;
            } else if (angle == 180) {
                dir_x = -dir_x;
                dir_y = -dir_y;
            } else if (angle == 270) {
                int temp = dir_x;
                dir_x = -dir_y;
                dir_y = temp;
            }
            break;

        case 'F':
            x += dir_x * a.val;
            y += dir_y * a.val;
            break;

        default:
            break;
        }
    }

    return abs(x) + abs(y);
}

static int solution_for_puzzle_2(const std::vector<action_t>& actions)
{
    int ship_x = 0, ship_y = 0;
    // x, y now are for waypoint
    int x = 10, y = 1;

    for (auto a: actions) {
        int angle;

        switch (a.act) {
        case 'N':
            y += a.val;
            break;

        case 'S':
            y -= a.val;
            break;;

        case 'E':
            x += a.val;
            break;

        case 'W':
            x -= a.val;
            break;

        case 'L':
            if (a.val == 90)
                angle = 270;
            else if (a.val == 180)
                angle = 180;
            else if (a.val == 270)
                angle = 90;
            else
                angle = 0;
            // fall through
        case 'R':
            if (a.act == 'R')
                angle = a.val;

            if (angle == 90) {
                int temp = x;
                x = y;
                y = -temp;
            } else if (angle == 180) {
                x = -x;
                y = -y;
            } else if (angle == 270) {
                int temp = x;
                x = -y;
                y = temp;
            }
            break;

        case 'F':
            ship_x += x * a.val;
            ship_y += y * a.val;
            break;

        default:
            break;
        }

        //std::cout << a.act << a.val << ": " << ship_x << "," << ship_y << " waypoint: " << x << "," << y << "\n";
    }

    return abs(ship_x) + abs(ship_y);
}


int main()
{
    std::vector<action_t> actions;
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream is{line};        
        action_t a;
        is >> a.act >> a.val;
        actions.push_back(a);
        //std::cout << a.act << ":" << a.val << "\n";
    }

    int distance;

    distance = solution_for_puzzle_1(actions);
    std::cout << "distance is " << distance << "\n";
    assert(distance == 1133);

    distance = solution_for_puzzle_2(actions);
    std::cout << "distance is " << distance << "\n";
    assert(distance == 61053);

    return 0;
}
