#include "ec.hpp"
#include <iostream>

int main() {
    EC_point a(-7);
    EC_point b(-5);
    EC_point c = a + b;
    mpz_out_str(stdout, 10, c.x);
    std::cout << std::endl;
    mpz_out_str(stdout, 10, c.y);
    std::cout << std::endl;
}
