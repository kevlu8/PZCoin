#include "ec.hpp"

EC_point::EC_point() { mpz_inits(x, y, NULL); }

EC_point::EC_point(const mpz_t x, const mpz_t y) {
	mpz_init_set(this->x, x);
	mpz_init_set(this->y, y);
}

EC_point::EC_point(const int x, const int y) {
	mpz_init_set_si(this->x, x);
	mpz_init_set_si(this->y, y);
}

EC_point::EC_point(const mpz_t x) {
	mpz_init_set(this->x, x);
	mpz_t temp;
	mpz_inits(this->y, temp, NULL);
	mpz_pow_ui(this->y, x, 3);
	mpz_pow_ui(temp, x, 2);
	mpz_addmul_ui(this->y, temp, A);
	mpz_add(this->y, this->y, this->x);
	mpz_sqrt(this->y, this->y);
	mpz_clear(temp);
}

EC_point::EC_point(const int x) {
	mpz_init_set_si(this->x, x);
	mpz_t temp;
	mpz_inits(this->y, temp, NULL);
	mpz_pow_ui(this->y, this->x, 3);
	mpz_pow_ui(temp, this->x, 2);
	mpz_addmul_ui(this->y, temp, A);
	mpz_add(this->y, this->y, this->x);
	mpz_sqrt(this->y, this->y);
	mpz_clear(temp);
}

EC_point::~EC_point() {
	// mpz_clears(x, y, NULL);
}

EC_point EC_point::operator+(EC_point point) {
	if (mpz_cmp(this->x, point.x) < 0) {
		return point + *this;
	}
	if (mpz_cmp(this->x, point.x) + mpz_cmp(this->y, point.y) == 0) {
		return this->double_point();
	}
	mpq_t slope, temp2, intercept, accy1, accy2;
	mpz_t temp, resx, resy;
	mpq_inits(slope, temp2, intercept, accy1, accy2, NULL);
	mpz_inits(temp, resx, resy, NULL);
	// Calculate y values with more precision
	mpz_pow_ui(mpq_numref(accy1), this->x, 3);
	mpz_set_ui(mpq_denref(accy1), 1);
	mpz_pow_ui(temp, this->x, 2);
	mpz_addmul_ui(mpq_numref(accy1), temp, A);
	mpz_add(mpq_numref(accy1), mpq_numref(accy1), this->x);
	mpq_set_d(accy1, sqrt(mpq_get_d(accy1)));
	if (mpz_cmp_si(this->y, 0) < 0)
		mpq_neg(accy1, accy1);
	mpz_pow_ui(mpq_numref(accy2), point.x, 3);
	mpz_set_ui(mpq_denref(accy2), 1);
	mpz_pow_ui(temp, point.x, 2);
	mpz_addmul_ui(mpq_numref(accy2), temp, A);
	mpz_add(mpq_numref(accy2), mpq_numref(accy2), point.x);
	mpq_set_d(accy2, sqrt(mpq_get_d(accy2)));
	if (mpz_cmp_si(point.y, 0) < 0)
		mpq_neg(accy2, accy2);
	// Calculate the numerator of the slope
	mpq_sub(slope, accy1, accy2);
	// Calculate the denominator of the slope
	mpz_sub(temp, this->x, point.x);
	mpz_mul(mpq_denref(slope), mpq_denref(slope), temp);
	// Calculate intercept (b of mx + b)
	mpq_canonicalize(slope);
	mpz_mul(mpq_numref(intercept), mpq_numref(slope), x);
	mpz_set(mpq_denref(intercept), mpq_denref(slope));
	mpq_neg(intercept, intercept);
	mpq_add(intercept, intercept, accy1);
	mpq_canonicalize(intercept);
	// Calculate point of intersection
	// Equation: x3 = m^2 - A - x2 - x1
	mpz_pow_ui(mpq_numref(temp2), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(temp2), mpq_denref(slope), 2);
	mpz_submul_ui(mpq_numref(temp2), mpq_denref(temp2), A);
	mpz_submul(mpq_numref(temp2), mpq_denref(temp2), this->x);
	mpz_submul(mpq_numref(temp2), mpq_denref(temp2), point.x);
	mpz_set_q(resx, temp2);
	// Calculate y value
	mpq_mul(slope, slope, temp2);
	mpq_add(slope, slope, intercept);
	mpz_set_q(resy, slope);
	mpz_neg(resy, resy);
	EC_point result = EC_point(resx, resy);
	// Clear memory
	mpz_clears(temp, resx, resy, NULL);
	mpq_clears(slope, temp2, intercept, NULL);
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
	return result;
}

EC_point EC_point::double_point() {
	mpq_t slope, temp2, intercept;
	mpz_t temp, resx, resy;
	// Calculate the numerator of the slope using the formula (derivative of elliptic curve)
	mpq_inits(slope, temp2, intercept, NULL);
	mpz_inits(temp, resx, resy, NULL);
	mpz_pow_ui(mpq_numref(slope), x, 2);
	mpz_mul_ui(mpq_numref(slope), mpq_numref(slope), 3);
	mpz_addmul_ui(mpq_numref(slope), x, 2 * A);
	mpz_add_ui(mpq_numref(slope), mpq_numref(slope), 1);
	// Calculate the denominator of the slope using the formula (derivative of elliptic curve)
	mpz_pow_ui(mpq_denref(slope), x, 3);
	mpz_pow_ui(temp, x, 2);
	mpz_addmul_ui(mpq_denref(slope), temp, A);
	mpz_add(mpq_denref(slope), mpq_denref(slope), x);
	mpq_set_d(temp2, sqrt(mpz_get_d(mpq_denref(slope))));
	mpz_set_ui(mpq_denref(slope), 2);
	mpz_mul(mpq_denref(slope), mpq_denref(slope), mpq_numref(temp2));
	mpz_mul(mpq_numref(slope), mpq_numref(slope), mpq_denref(temp2));
	// Calculate intercept (b of mx + b)
	mpz_mul(mpq_numref(intercept), mpq_numref(slope), x);
	mpz_set(mpq_denref(intercept), mpq_denref(slope));
	mpq_neg(intercept, intercept);
	mpz_pow_ui(mpq_numref(temp2), x, 3);
	mpz_set_ui(mpq_denref(temp2), 1);
	mpz_pow_ui(temp, x, 2);
	mpz_addmul_ui(mpq_numref(temp2), temp, A);
	mpz_add(mpq_numref(temp2), mpq_numref(temp2), x);
	mpq_set_d(temp2, sqrt(mpq_get_d(temp2)));
	if (mpz_cmp_si(y, 0) < 0)
		mpq_neg(temp2, temp2);
	mpq_add(intercept, intercept, temp2);
	// Calculate point of intersection
	// Equation: x2 = m^2 - A - 2*x1
	mpz_pow_ui(mpq_numref(temp2), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(temp2), mpq_denref(slope), 2);
	mpz_submul_ui(mpq_numref(temp2), mpq_denref(temp2), A);
	mpz_submul(mpq_numref(temp2), mpq_denref(temp2), x);
	mpz_submul(mpq_numref(temp2), mpq_denref(temp2), x);
	mpz_set_q(resx, temp2);
	// Calculate y value
	mpq_mul(slope, slope, temp2);
	mpq_add(slope, slope, intercept);
	mpz_set_q(resy, slope);
	mpz_neg(resy, resy);
	EC_point result = EC_point(resx, resy);
	// Clear memory
	mpz_clears(temp, resx, resy, NULL);
	mpq_clears(slope, temp2, intercept, NULL);
	return result;
}

std::string hash(std::string message, std::string key) { return NULL; }
