// TODO: Implement modulo arithmetic for the field

#include "ec.hpp"

void calc_y(const mpq_t x, mpq_t y, const bool neg) {
	mpq_t temp;
	mpq_init(temp);
	// y = x^3
	mpz_pow_ui(mpq_numref(y), mpq_numref(x), 3);
	mpz_pow_ui(mpq_denref(y), mpq_denref(x), 3);
	// temp = x^2 / 4
	mpz_pow_ui(mpq_numref(temp), mpq_numref(x), 2);
	mpz_pow_ui(mpq_denref(temp), mpq_denref(x), 2);
	mpz_mul_ui(mpq_denref(temp), mpq_denref(temp), 4);
	// y = x^3 + x^2 / 4
	mpq_add(y, y, temp);
	// y = x^3 + x^2 / 4 + B
	mpz_addmul_ui(mpq_numref(y), mpq_denref(y), B);
	// y = sqrt(x^3 + x^2 / 4 + B)
	mpq_set_d(y, sqrt(mpq_get_d(y)));
	if (neg)
		mpq_neg(y, y);
	// temp = x/2
	mpz_set(mpq_numref(temp), mpq_numref(x));
	mpz_mul_ui(mpq_denref(temp), mpq_denref(x), 2);
	mpq_sub(y, y, temp);
	mpq_canonicalize(y);
	mpq_clear(temp);
}

EC_point::EC_point(const mpq_t x) {
	mpq_init(this->x);
	mpq_set(this->y, x);
	mpq_init(y);
	calc_y(this->x, y, false);
}

EC_point::EC_point(const double x) {
	mpq_init(this->x);
	mpq_set_d(this->x, x);
	mpq_init(y);
	calc_y(this->x, y, false);
}

EC_point::EC_point(const mpq_t x, const bool neg) {
	mpq_init(this->x);
	mpq_set(this->x, x);
	mpq_init(y);
	calc_y(this->x, y, neg);
}

EC_point::EC_point(const double x, const bool neg) {
	mpq_init(this->x);
	mpq_set_d(this->x, x);
	mpq_init(y);
	calc_y(this->x, y, neg);
}

EC_point::~EC_point() {
	// mpz_clear(x);
	// mpq_clear(y);
}

EC_point::EC_point(const mpq_t x, const mpq_t y) {
	mpq_inits(this->x, this->y, NULL);
	mpq_set(this->x, x);
	mpq_set(this->y, y);
}

EC_point EC_point::operator+(EC_point point) {
	// For some reason if this isn't here the program crashes
	if (mpq_cmp(x, point.x) < 0) {
		return point + *this;
	}
	// If the points are the same, return the double of the point
	if (mpq_cmp(x, point.x) + mpq_cmp(y, point.y) == 0) {
		return this->double_point();
	}
	mpq_t slope, intercept, temp, resx, resy;
	mpq_inits(slope, intercept, temp, resx, resy, NULL);
	// slope = (y2 - y1)
	mpq_sub(slope, y, point.y);
	// temp = (x2 - x1)
	mpq_sub(temp, x, point.x);
	// slope = (y2 - y1) / (x2 - x1)
	mpq_div(slope, slope, temp);
	mpq_canonicalize(slope);
	// intercept = slope * x1
	mpq_mul(intercept, slope, x);
	// intercept = y1 - slope * x1
	mpq_sub(intercept, y, intercept);
	mpq_canonicalize(intercept);
	// resx = m^2
	mpz_pow_ui(mpq_numref(resx), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(resx), mpq_denref(slope), 2);
	// resx = m^2 + m
	mpq_add(resx, resx, slope);
	// resx = m^2 + m - x1
	mpq_sub(resx, resx, x);
	// resx = m^2 + m - 2x1
	mpq_sub(resx, resx, point.x);
	// y = mx
	mpq_mul(resy, slope, resx);
	// y = mx + b
	mpq_add(resy, resy, intercept);
	// reflect across x axis -(y + x)
	mpq_add(resy, resy, resx);
	mpq_neg(resy, resy);
	mpq_canonicalize(resy);
	// Create the point
	EC_point result(resx, resy);
	// Clean up
	mpq_clears(slope, intercept, temp, resx, resy, NULL);
	return result;
}

void EC_point::operator+=(EC_point point) {
	EC_point result = *this + point;
	mpq_set(x, result.x);
	mpq_set(y, result.y);
}

EC_point EC_point::operator*(mpz_t n) {
	if (mpz_cmp_si(n, 0) == 0) {
		return EC_point(0.0);
	}
	if (mpz_cmp_si(n, 1) == 0) {
		return *this;
	}
	// Implement the double-and-add algorithm (based on square-multiply algorithm)
	EC_point result = EC_point(x, mpq_cmp(y, 0) < 0);
	for (int i = mpz_sizeinbase(n, 2) - 2; i >= 0; i--) {
		result = result.double_point();
		if (mpz_tstbit(n, i)) {
			result += EC_point(x, y);
		}
	}
	return result;
}

EC_point EC_point::double_point() {
	mpq_t slope, intercept, temp, resx, resy;
	mpq_inits(slope, intercept, temp, resx, resy, NULL);
	// slope = 3x^2
	mpq_mul(slope, x, x);
	mpz_mul_ui(mpq_numref(slope), mpq_numref(slope), 3);
	// slope = 3x^2 - y
	mpq_sub(slope, slope, y);
	// temp = 2y
	mpq_mul_2exp(temp, y, 1);
	// temp = 2y + x
	mpq_add(temp, temp, x);
	// slope = (3x^2 - y) / (2y + x)
	mpq_div(slope, slope, temp);
	mpq_canonicalize(slope);
	// intercept = slope * x1
	mpq_mul(intercept, slope, x);
	// intercept = y1 - slope * x1
	mpq_sub(intercept, y, intercept);
	mpq_canonicalize(intercept);
	// resx = m^2
	mpz_pow_ui(mpq_numref(resx), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(resx), mpq_denref(slope), 2);
	// resx = m^2 + m
	mpq_add(resx, resx, slope);
	// resx = m^2 + m - x1
	mpq_sub(resx, resx, x);
	// resx = m^2 + m - 2x1
	mpq_sub(resx, resx, x);
	// y = mx
	mpq_mul(resy, slope, resx);
	// y = mx + b
	mpq_add(resy, resy, intercept);
	// reflect across x axis -(y + x)
	mpq_add(resy, resy, resx);
	mpq_neg(resy, resy);
	mpq_canonicalize(resy);
	EC_point result(resx, resy);
	// Clean up
	mpq_clears(slope, intercept, temp, resx, resy, NULL);
	return result;
}

char *EC_point::print_point(FILE *out) {
	if (out != NULL) {
		fprintf(out, "(%f, %f)\n", mpq_get_d(this->x), mpq_get_d(this->y));
		return NULL;
	} else {
		char *buf = (char *)malloc(sizeof(char) * 100);
		sprintf(buf, "(%f, %f)\n", mpq_get_d(this->x), mpq_get_d(this->y));
		buf = (char *)realloc(buf, strlen(buf) + 1);
		return buf;
	}
}

std::string hash(std::string message, std::string key) { return NULL; }
