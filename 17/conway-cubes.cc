#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


struct coord_t {
    int x;
    int y;
    int z;
};

bool operator< (const coord_t co1, const coord_t co2) {
    if (co1.x < co2.x)
        return true;
    else if (co1.x > co2.x)
        return false;
    else if (co1.y < co2.y)
        return true;
    else if (co1.y > co2.y)
        return false;
    else if (co1.z < co2.z)
        return true;
    else
        return false;
}

struct active_limit_t {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int z_min;
    int z_max;
};

static void update_active_limit(active_limit_t& al, int x, int y, int z)
{
    if (x < al.x_min)
        al.x_min = x;
    if (x > al.x_max)
        al.x_max = x;

    if (y < al.y_min)
        al.y_min = y;
    if (y > al.y_max)
        al.y_max = y;

    if (z < al.z_min)
        al.z_min = z;
    if (z > al.z_max)
        al.z_max = z;
}

static void update_active_limit(active_limit_t& al, coord_t co)
{
    update_active_limit(al, co.x, co.y, co.z);
}

void extend_cubes(std::map<coord_t, char>& cubes, active_limit_t al)
{
    for (int x = al.x_min - 1; x <= al.x_max + 1; x++)
        for (int y = al.y_min - 1; y <= al.y_max + 1; y++) {
            coord_t co;
            co.x = x; co.y = y;
            co.z = al.z_min - 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
            co.z = al.z_max + 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
        }

    for (int y = al.y_min - 1; y <= al.y_max + 1; y++)
        for (int z = al.z_min - 1; z <= al.z_max + 1; z++) {
            coord_t co;
            co.y = y; co.z = z;
            co.x = al.x_min - 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
            co.x = al.x_max + 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
        }

    for (int x = al.x_min - 1; x <= al.x_max + 1; x++)
        for (int z = al.z_min - 1; z <= al.z_max + 1; z++) {
            coord_t co;
            co.x = x; co.z = z;
            co.y = al.y_min - 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
            co.y = al.y_max + 1;
            if (cubes.find(co) == cubes.end())
                cubes[co] = '.';
        }
}

static int active_neighbors(std::map<coord_t, char>& cubes, coord_t co)
{
    int active = 0;

    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            for (int z = -1; z <= 1; z++) {
                coord_t nb;
                nb.x = co.x + x;
                nb.y = co.y + y;
                nb.z = co.z + z;
                auto nb_cube = cubes.find(nb);
                if (nb_cube != cubes.end() && nb_cube->second == '#')
                    active++;
            }

    return active - (cubes[co] == '#' ? 1 : 0);
}

static int solution_for_puzzle_1(std::map<coord_t, char>& cubes, active_limit_t al, int cycles)
{
    int cycle = 0;

    while (cycle < cycles) {
        extend_cubes(cubes, al);

        std::map<coord_t, char> cubes_next;

        for (auto cube: cubes) {
            int n = active_neighbors(cubes, cube.first);

            if (cube.second == '#' && n != 2 && n != 3)
                cubes_next[cube.first] = '.';
            else if (cube.second == '.' && n == 3) {
                cubes_next[cube.first] = '#';
                update_active_limit(al, cube.first);
            } else
                cubes_next[cube.first] = cube.second;
        }

        cubes = std::move(cubes_next);

        cycle++;
    }

    int active = 0;
    for (auto cube: cubes)
        if (cube.second == '#')
            active++;

    return active;
}


int main()
{
    std::ifstream input_file{"input"};

    if (!input_file) {
        std::cout << "input file is missing\n";
        return -1;
    }

    std::vector<std::string> input;
    std::map<coord_t, char> cubes;

    std::string line;
    while (getline(input_file, line))
        input.push_back(line);

    active_limit_t al;
    al.x_min = 0;
    al.x_max = 0;
    al.y_min = 0;
    al.y_max = 0;
    al.z_min = 0;
    al.z_max = 0;

    for (int i = 0; i < input.size(); i++)
        for (int j = 0; j < input[0].size(); j++) {
            coord_t co;
            co.x = i;
            co.y = j;
            co.z = 0;
            cubes[co] = input[i][j];
            if (input[i][j] == '#')
                update_active_limit(al, i, j, 0);
        }


    int num_active;
    num_active = solution_for_puzzle_1(cubes, al, 6);
    std::cout << "active cubles after 6: " << num_active << "\n";

    return 0;
}
