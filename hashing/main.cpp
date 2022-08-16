#include "ec.hpp"

int main() {
	double ax, bx;
	std::cout << "Enter x coordinate of point A: ";
	std::cin >> ax;
	std::cout << "Enter x coordinate of point B: ";
	std::cin >> bx;
	EC_point a(ax);
	EC_point b(bx);
	std::cout << "a: " << a.print_point(NULL);
	std::cout << "b: " << b.print_point(NULL);
	EC_point c = a + b;
	std::cout << "a + b: " << c.print_point(NULL);
}
