#include "ec.hpp"

int main() {
	std::string ax;
	std::cout << "Enter x coordinate of point A: ";
	std::cin >> ax;
	EC_init();
	mpz_t aval;
	mpz_init_set_str(aval, ax.c_str(), 0);
	EC_point a = EC_point(aval, true);
	std::cout << "a: " << a.print_point(NULL, 16) << std::endl;
	a = EC_point(aval, true);
	std::cout << "a: " << a.print_point(NULL, 16) << std::endl;
}
