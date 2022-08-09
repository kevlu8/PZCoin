#include "ec.hpp"

EC_point::EC_point() { mpz_inits(x, y, NULL); }

EC_point::EC_point(const mpz_t x, const mpz_t y) {
	mpz_init_set(this->x, x);
	mpz_init_set(this->y, y);
}

EC_point::~EC_point() { mpz_clears(x, y, NULL); }

EC_point EC_point::operator+(EC_point point) {
	if (mpz_cmp(this->x, point.x) + mpz_cmp(this->y, point.y) == 0) {
		return this->double_point();
	}
	mpq_t slope;
	mpz_t temp, resx, resy, intercept;
	mpq_init(slope);
	// Calculate the numerator of the slope
	mpz_sub(temp, this->y, point.y);
	// Calculate the denominator of the slope
	mpz_sub(temp, this->x, point.x);
	mpq_canonicalize(slope);
	// Calculate intercept (b of mx + b)
	mpz_init_set(intercept, y);
	mpz_submul(intercept, mpq_numref(slope), x);
	mpz_tdiv_q(intercept, intercept, mpq_denref(slope));
	// Calculate point of intersection
	// Equation: x3 = m^2 - A - x2 - x1
	mpz_init_set(resx, mpq_numref(slope));
	mpz_pow_ui(resx, resx, 2);
	mpz_sub(resx, resx, x);
	mpz_sub(resx, resx, point.x);
	mpz_sub_ui(resx, resx, 486662);
	// Calculate y value
	mpz_init(resy);
	mpz_addmul(resy, resx, mpq_numref(slope));
	mpz_tdiv_q(resy, resy, mpq_denref(slope));
	mpz_add(resy, resy, intercept);
	EC_point result = EC_point(resx, resy);
	// Clear memory
	mpz_clears(temp, resx, resy, intercept, NULL);
	mpq_clear(slope);
	return result;
}

void EC_point::operator+=(EC_point point) { *this = *this + point; }

EC_point EC_point::operator*(mpz_t n) {
	// Implement the double-and-add algorithm
	EC_point result = EC_point(x, y);
	for (int i = mpz_sizeinbase(n, 2) - 2; i >= 0; i--) {
		result = result.double_point();
		if (mpz_tstbit(n, i)) {
			result += EC_point(x, y);
		}
	}
}

EC_point EC_point::double_point() {
	mpq_t slope;
	mpz_t temp, resx, resy, intercept;
	// Calculate the numerator of the slope using the formula (derivative of elliptic curve)
	mpq_init(slope);
	mpz_pow_ui(mpq_numref(slope), x, 2);
	mpz_mul_ui(mpq_numref(slope), mpq_numref(slope), 3);
	mpz_addmul_ui(mpq_numref(slope), x, 973324);
	mpz_add_ui(mpq_numref(slope), mpq_numref(slope), 1);
	// Calculate the denominator of the slope using the formula (derivative of elliptic curve)
	mpz_pow_ui(mpq_denref(slope), x, 3);
	mpz_init(temp);
	mpz_pow_ui(temp, x, 2);
	mpz_addmul_ui(mpq_denref(slope), temp, 486662);
	mpz_add(mpq_denref(slope), mpq_denref(slope), x);
	mpz_sqrt(mpq_denref(slope), mpq_denref(slope));
	mpz_mul_ui(mpq_denref(slope), mpq_denref(slope), 2);
	mpq_canonicalize(slope);
	// Calculate intercept (b of mx + b)
	mpz_init_set(intercept, y);
	mpz_submul(intercept, mpq_numref(slope), x);
	mpz_tdiv_q(intercept, intercept, mpq_denref(slope));
	// Calculate point of intersection
	// Equation: x2 = m^2 - A - 2*x1
	mpz_init_set(resx, mpq_numref(slope));
	mpz_pow_ui(resx, resx, 2);
	mpz_submul_ui(resx, x, 2);
	mpz_sub_ui(resx, resx, 486662);
	// Calculate y value
	mpz_init_set(resy, intercept);
	mpz_addmul(resy, mpq_numref(slope), resx);
	EC_point result = EC_point(resx, resy);
	// Clear memory
	mpz_clears(temp, resx, resy, intercept, NULL);
	mpq_clear(slope);
	return result;
}
