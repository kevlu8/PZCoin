#include "ec.hpp"
#include "ecoh.hpp"

int main() {
	int ia, ib;
	std::cout << "Enter a: ";
	std::cin >> ia;
	std::cout << "Enter b: ";
	std::cin >> ib;
	gf_init();
	mpz_t temp, a, b;
	mpz_init_set_si(a, ia);
	mpz_init_set_si(b, ib);
	mpz_init(temp);
	gf_add(temp, a, b);
	std::cout << "a + b = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_mul(temp, a, b);
	std::cout << "a * b = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_div(temp, a, b);
	std::cout << "a / b = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_pow(temp, a, b);
	std::cout << "a ^ b = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_inv(temp, a);
	std::cout << "a ^ -1 = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_mod(temp, a);
	std::cout << "a mod p = " << mpz_get_str(NULL, 10, temp) << std::endl;
	gf_sqrt(temp, a);
	std::cout << "sqrt(a) = " << mpz_get_str(NULL, 10, temp) << std::endl;
	mpz_clear(temp);
	mpz_clear(a);
	mpz_clear(b);
	return 0;
}
