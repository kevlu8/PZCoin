#include "ec.hpp"

int main() {
	double ax, bx;
	std::cout << "Enter x coordinate of point A: ";
	std::cin >> ax;
	std::cout << "Enter y coordinate of point B: ";
	std::cin >> bx;
	EC_point a(ax);
	EC_point b(bx);
	std::cout << "a: " << mpq_get_d(a.x) << ' ' << mpq_get_d(a.y) << '\n';
	std::cout << "b: " << mpq_get_d(b.x) << ' ' << mpq_get_d(b.y) << '\n';
	EC_point c = a + b;
	std::cout << "main: " << mpq_get_d(c.x) << ' ' << mpq_get_d(c.y) << std::endl;
}
