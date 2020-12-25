#include <iostream>

int main ()
{
    // 20201227 is prime number

    /* test
    unsigned long long p1 = 5764801;
    unsigned long long p2 = 17807724;
    */
    unsigned long long p1 = 8987316;
    unsigned long long p2 = 14681524;

    unsigned long long value = 1;
    unsigned long long subject_number = 7;
    unsigned long long l1 = 0;
    do {
        value = value * subject_number % 20201227;
        l1++;
    } while (value != p1);
    std::cout << "l1 = " << l1 << "\n";

    value = 1;
    subject_number = p2;
    for (unsigned long long i = 0; i < l1; i++)
        value = value * subject_number % 20201227;

    std::cout << "encryption key: " << value << "\n";

    return 0;
}
