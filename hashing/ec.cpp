#include "ec.hpp"

void calc_y(const mpz_t x, mpq_t y) {
	mpq_t temp;
	mpq_init(temp);
	// y = x^3 / 1
	mpz_pow_ui(mpq_numref(y), x, 3);
	// temp = x^2 / 4
	mpz_pow_ui(mpq_numref(temp), x, 2);
	mpz_set_ui(mpq_denref(temp), 4);
	// y = x^3 + x^2 / 4
	mpq_add(y, y, temp);
	// y = x^3 + x^2 / 4 + B
	mpz_addmul_ui(mpq_numref(y), mpq_denref(y), B);
	// y = sqrt(x^3 + x^2 / 4 + B)
	mpq_set_d(y, sqrt(mpq_get_d(y)));
	mpz_set_ui(mpq_denref(temp), 2);
	mpq_sub(y, y, temp);
	mpq_canonicalize(y);
	mpq_clear(temp);
}

void calc_y(const mpq_t x, mpq_t y) {
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
	mpz_set_ui(mpq_denref(temp), 2);
	mpq_sub(y, y, temp);
	mpq_canonicalize(y);
	mpq_clear(temp);
}

EC_point::EC_point(const mpz_t x) {
	mpz_init_set(this->x, x);
	mpq_init(y);
	calc_y(this->x, y);
}

EC_point::EC_point(const int x) {
	mpz_init_set_si(this->x, x);
	mpq_init(y);
	calc_y(this->x, y);
}

EC_point::EC_point(const mpz_t x, const bool neg) {
	mpz_init_set(this->x, x);
	mpq_init(y);
	calc_y(this->x, y);
	if (neg)
		mpq_neg(y, y);
}

EC_point::EC_point(const int x, const bool neg) {
	mpz_init_set_si(this->x, x);
	mpq_init(y);
	calc_y(this->x, y);
	if (neg)
		mpq_neg(y, y);
}

EC_point::~EC_point() {
	// mpz_clear(x);
	// mpq_clear(y);
}

EC_point::EC_point(const mpz_t x, const mpq_t y) {
	mpz_init_set(this->x, x);
	mpq_init(this->y);
	mpq_set(this->y, y);
}

EC_point EC_point::operator+(EC_point point) {
	// For some reason if this isn't here the program crashes
	if (mpz_cmp(x, point.x) < 0) {
		return point + *this;
	}
	// If the points are the same, return the double of the point
	if (mpz_cmp(x, point.x) + mpq_cmp(y, point.y) == 0) {
		return this->double_point();
	}
	mpq_t slope, intercept, temp, resy;
	mpz_t resx;
	mpq_inits(slope, intercept, temp, resy, NULL);
	mpz_init(resx);
	// slope = (y2 - y1) / 1
	mpq_sub(slope, y, point.y);
	// temp = (x2 - x1) / 1
	mpz_sub(mpq_numref(temp), x, point.x);
	// slope = (y2 - y1) / (x2 - x1)
	mpq_div(slope, slope, temp);
	mpq_canonicalize(slope);
	// intercept = y1 - slope * x1
	mpz_set(mpq_denref(temp), mpq_denref(slope));
	mpz_mul(mpq_numref(temp), mpq_numref(slope), x);
	mpq_sub(intercept, y, temp);
	mpq_canonicalize(intercept);
	// temp = m^2
	mpz_pow_ui(mpq_numref(temp), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(temp), mpq_denref(slope), 2);
	// temp = m^2 + m
	mpq_add(temp, temp, slope);
	// temp = m^2 + m - x1
	mpz_submul(mpq_numref(temp), x, mpq_denref(temp));
	// temp = m^2 + m - x1 - x2
	mpz_submul(mpq_numref(temp), point.x, mpq_denref(temp));
	// Calculate the y coordinate of the point
	calc_y(temp, resy);
	// Debug print
	std::cout << "plus: " << mpq_get_str(NULL, 10, temp) << ' ' << mpq_get_str(NULL, 10, resy) << std::endl;
	// Create the point
	mpz_set_q(resx, temp);
	EC_point result(resx, resy);
	// Clean up
	mpq_clears(slope, intercept, temp, resy, NULL);
	mpz_clear(resx);
	return result;
}

void EC_point::operator+=(EC_point point) {
	EC_point result = *this + point;
	mpz_set(x, result.x);
	mpq_set(y, result.y);
}

EC_point EC_point::operator*(mpz_t n) {
	if (mpz_cmp_si(n, 0) == 0) {
		return EC_point(0);
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
	mpq_t slope, intercept, temp, resy;
	mpz_t resx;
	mpq_inits(slope, intercept, temp, resy, NULL);
	mpz_init(resx);
	// slope = x^2
	mpz_pow_ui(mpq_numref(slope), x, 2);
	// slope = 6x^2
	mpz_mul_ui(mpq_numref(slope), mpq_numref(slope), 6);
	// slope = 6x^2 + x
	mpz_add(mpq_numref(slope), mpq_numref(slope), x);
	// temp = x^3
	mpz_pow_ui(mpq_numref(temp), x, 3);
	// temp = x^3 + B
	mpz_add_ui(mpq_numref(temp), mpq_numref(temp), B);
	// temp = 4x^3 + 4B
	mpz_mul_ui(mpq_numref(temp), mpq_numref(temp), 4);
	// save value of temp in denominator of slope
	mpz_set(mpq_denref(slope), mpq_numref(temp));
	// temp = x^2
	mpz_pow_ui(mpq_numref(temp), x, 2);
	// retrieve stored value of temp in denominator of slope
	mpz_set(mpq_numref(temp), mpq_denref(slope));
	// temp = sqrt(4x^3 + x^2 + 4B)
	mpq_set_d(temp, sqrt(mpq_get_d(temp)));
	// temp = sqrt(4x^3 + x^2 + 4B) * 2
	mpz_mul_ui(mpq_numref(temp), mpq_numref(temp), 2);
	// temp = 1/(2sqrt(4x^3 + x^2 + 4B))
	mpq_inv(temp, temp);
	// slope = (6x^2 + x)/(2sqrt(4x^3 + x^2 + 4B))
	mpq_mul(slope, slope, temp);
	// temp = 1/2
	mpz_set_ui(mpq_numref(temp), 1);
	mpz_set_ui(mpq_denref(temp), 2);
	// slope = (6x^2 + x)/(2sqrt(4x^3 + x^2 + 4B)) - 1/2
	mpq_sub(slope, slope, temp);
	mpq_canonicalize(slope);
	// intercept = y1 - slope * x1
	mpz_set(mpq_denref(temp), mpq_denref(slope));
	mpz_mul(mpq_numref(temp), mpq_numref(slope), x);
	mpq_sub(intercept, y, temp);
	mpq_canonicalize(intercept);
	// temp = m^2
	mpz_pow_ui(mpq_numref(temp), mpq_numref(slope), 2);
	mpz_pow_ui(mpq_denref(temp), mpq_denref(slope), 2);
	// temp = m^2 + m
	mpq_add(temp, temp, slope);
	// temp = m^2 + m - x1
	mpz_submul(mpq_numref(temp), x, mpq_denref(temp));
	// temp = m^2 + m - 2x1
	mpz_submul(mpq_numref(temp), x, mpq_denref(temp));
	// Calculate the y coordinate of the point
	calc_y(temp, resy);
	// Debug print
	std::cout << "double: " << mpq_get_str(NULL, 10, temp) << ' ' << mpq_get_str(NULL, 10, resy) << std::endl;
	// Create the point
	mpz_set_q(resx, temp);
	EC_point result(resx, resy);
	// Clean up
	mpq_clears(slope, intercept, temp, resy, NULL);
	mpz_clear(resx);
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
