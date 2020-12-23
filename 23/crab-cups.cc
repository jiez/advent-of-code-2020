#include <cassert>
#include <string>
#include <iostream>
#include <unordered_map>

// NEXT points to the clockwise next cup
// PREV points to the counter-clockwise next cup
struct cup_t {
    int label;
    cup_t *prev;
    cup_t *next;
};

struct cup_circle_t {
    int max;
    int min;
    cup_t *current;
    std::unordered_map<int, cup_t *> lookup_table;

    cup_circle_t(): max{-1}, min{-1}, current{nullptr} {}
    ~cup_circle_t() {
        if (current == nullptr)
            return;

        cup_t *p = current;
        do {
            cup_t *q = p->next;
            delete p;
            p = q;
        } while (p != current);

        current = nullptr;
    }

    // add cup N before CURRENT
    void add(int n) {
        cup_t *t = new cup_t;
        t->label = n;
        if (current == nullptr) {
            t->prev = t;
            t->next = t;
            current = t;
        } else {
            cup_t *p = current->prev;
            p->next = t;
            t->prev = p;
            t->next = current;
            current->prev = t;
        }
        assert(lookup_table.find(n) == lookup_table.end());
        lookup_table[n] = t;
        if (min == -1 || n < min)
            min = n;
        if (max == -1 || n > max)
            max = n;
    }

    bool find(cup_t *from, size_t len, int cup) {
        cup_t *p = from;
        for (int i = 0; i < len; i++) {
            if (p->label == cup)
                return true;
            p = p->next;
        }
        return false;
    }

    void one_move() {
        int current_cup = current->label;
        int dest_cup = current_cup;
        do {
            dest_cup--;
            if (dest_cup < min)
                dest_cup = max;
        } while (find(current->next, 3, dest_cup));
        cup_t *dest = lookup_table.find(dest_cup)->second;
        cup_t *three_cups_first = current->next;
        cup_t *three_cups_last = three_cups_first->next->next;

        // take out 3 cups
        current->next = three_cups_last->next;
        three_cups_last->next->prev = current;

        // insert 3 cups after dest
        three_cups_last->next = dest->next;
        dest->next->prev = three_cups_last;
        dest->next = three_cups_first;
        three_cups_first->prev = dest;

        // move current to the next
        current = current->next;
    }

    void set_current(int cup) {
        assert(cup >= min && cup <= max);
        current = lookup_table[cup];
    }

    void move_current_next() {
        current = current->next;
    }

    int get_current_cup() {
        return current->label;
    }

    // print cups from current cup, which is excluded
    void print_cups() {
        cup_t *p = current->next;
        while (p != current) {
            std::cout << p->label;
            p = p->next;
        }
    }
};

static void solution_for_puzzle_1(cup_circle_t* circle)
{
    for (int i = 0; i < 100; i++)
        circle->one_move();
}

static void solution_for_puzzle_2(cup_circle_t* circle)
{
    for (int i = 0; i < 10000000; i++)
        circle->one_move();
}

int main()
{
    const std::string input{"624397158"};
    // below is the test in the puzzle
    //const std::string input{"389125467"};

    cup_circle_t circle;
    for (auto c: input)
        circle.add(c - '0');

    solution_for_puzzle_1(&circle);

    circle.set_current(1);
    std::cout << "result is ";
    circle.print_cups();
    std::cout << "\n";

    // part 2
    unsigned long long result;

    cup_circle_t circle2;
    for (auto c: input)
        circle2.add(c - '0');
    for (int i = 10; i <= 1000000; i++)
        circle2.add(i);

    solution_for_puzzle_2(&circle2);

    circle2.set_current(1);
    circle2.move_current_next();
    result = circle2.get_current_cup();
    circle2.move_current_next();
    result *= circle2.get_current_cup();

    std::cout << "result is " << result << "\n";

    return 0;
}
