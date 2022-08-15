#include "ec.hpp"

int main() {
	EC_point a(-2);
	EC_point b(1);
	EC_point c = a + b;
	std::cout << "main: " << mpz_out_str(NULL, 10, c.x) << ' ' << mpq_out_str(NULL, 10, c.y) << std::endl;
}
