#include <cassert>
#include <string>
#include <iostream>


// assume cups will be labeled from 1 to N
struct cup_circle_t {
    int current;
    int current_prev;
    // next_cup[i] is the index of the clockwisely next cup. next_cup[0] is unused
    int *next_cup;
    int num_of_cups;

    cup_circle_t(const std::string& input, int num) {
        next_cup = new int[num + 1];
        num_of_cups = num;

        current = input[0] - '0';
        for (int i = 0; i < input.size() - 1; i++) {
            int cup = input[i] - '0';
            next_cup[cup] = input[i + 1] - '0';
        }

        int last_cup = input.back() - '0';

        if (input.size() == num) {
            next_cup[last_cup] = current;
            current_prev = last_cup;
            return;
        }

        next_cup[last_cup] = input.size() + 1;

        // add cups from input.size() + 1 to num
        for (int i = input.size() + 1; i <= num; i++)
            next_cup[i] = i + 1;

        next_cup[num] = current;
        current_prev = num;
    }

    ~cup_circle_t() {
        delete [] next_cup;
    }

    bool find(int from, size_t len, int cup) {
        int p = from;
        for (int i = 0; i < len; i++) {
            if (p == cup)
                return true;
            p = next_cup[p];
        }
        return false;
    }

    void one_move() {
        int current_cup = current;
        int dest_cup = current_cup;
        do {
            dest_cup--;
            if (dest_cup == 0)
                dest_cup = num_of_cups;
        } while (find(next_cup[current], 3, dest_cup));
        int three_cups_first = next_cup[current];
        int three_cups_last = next_cup[next_cup[three_cups_first]];

        // take out 3 cups
        next_cup[current] = next_cup[three_cups_last];

        // insert 3 cups after dest
        next_cup[three_cups_last] = next_cup[dest_cup];
        next_cup[dest_cup] = three_cups_first;

        // move current to the next
        current = next_cup[current];
    }
};

static void solution_for_puzzle_1(const std::string& input)
{
    size_t num = input.size();
    cup_circle_t circle(input, num);

    for (int i = 0; i < 100; i++)
        circle.one_move();

    std::cout << "result is ";
    size_t cup = 1;
    while (1) {
        cup = circle.next_cup[cup];
        if (cup == 1)
            break;
        std::cout << cup;
    }
    std::cout << "\n";
}

static void solution_for_puzzle_2(const std::string& input)
{
    size_t num = 1000000;
    cup_circle_t circle(input, num);

    for (int i = 0; i < 10000000; i++)
        circle.one_move();

    unsigned long long result;
    size_t cup = 1;
    cup = circle.next_cup[cup];
    result = cup;
    cup = circle.next_cup[cup];
    result *= cup;

    std::cout << "result is " << result << "\n";
}

int main()
{
    const std::string input{"624397158"};
    // below is the test in the puzzle
    //const std::string input{"389125467"};

    solution_for_puzzle_1(input);

    solution_for_puzzle_2(input);

    return 0;
}
