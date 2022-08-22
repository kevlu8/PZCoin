#include "ec.hpp"

mpz_t B, p, A;

void gf_sqrt(mpz_t x, mpz_t y) {
	// p ≡ 3 (mod 4)
	// mpz_t temp;
	// mpz_init(temp);
	// mpz_add_ui(temp, p, 1);
	// mpz_tdiv_q_2exp(temp, temp, 2);
	// mpz_powm(y, x, temp, p);
	// mpz_clear(temp);
	// p ≡ 5 (mod 8)
	mpz_t temp, v, i;
	mpz_inits(temp, v, i, NULL);
	// if (pow(y1, (p - 1) // 2, p) != 1) return NULL;
	mpz_sub_ui(temp, p, 1);
	mpz_tdiv_q_2exp(temp, temp, 1);
	mpz_powm_sec(y, y, temp, p);
	if (mpz_cmp_ui(y, 1) != 0) {
		mpz_set_si(y, -1);
		mpz_clears(temp, v, i, NULL);
		return;
	}
	// v = pow(2 * x, (p - 5) // 8, p)
	mpz_sub_ui(temp, p, 5);
	mpz_tdiv_q_2exp(temp, temp, 3);
	mpz_mul_2exp(x, x, 1);
	mpz_powm_sec(v, x, temp, p);
	// i = (2 * x * pow(v, 2, p)) % p
	mpz_powm_ui(temp, v, 2, p);
	mpz_mul(i, x, temp);
	mpz_mod(i, i, p);
	// return (x * v * (i - 1)) % p
	mpz_sub_ui(temp, i, 1);
	mpz_tdiv_q_2exp(x, x, 1);
	mpz_mul(y, x, v);
	mpz_mul(y, y, temp);
	mpz_mod(y, y, p);
	mpz_clears(temp, v, i, NULL);
}

void EC_init() {
	mpz_init_set_str(
		p,
		"80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000425",
		16);
	// Use A as a temporary variable
	mpz_init_set_str(
		A,
		"2f40e7e2221f295de297117b7f3d62f5c6a97ffcb8ceff1cd6ba8ce4a9a18ad84ffabbd8efa59332be7ad6756a66e294afd185a78ff12aa520e4de739baca0c7ffeff7f2955727a",
		16);
	// B = 864
	mpz_init_set_ui(B, 864);
	// B = 1/864
	mpz_invert(B, B, p);
	// B = 125/864
	mpz_mul_ui(B, B, 125);
	// B = 125 / 864 + B
	mpz_add(B, B, A);
	mpz_mod(B, B, p);
	// A = 48
	mpz_set_ui(A, 48);
	// A = 1/48
	mpz_invert(A, A, p);
	// A = -25/48
	mpz_mul_si(A, A, -25);
	mpz_mod(A, A, p);
}

void calc_y(const mpz_t x, mpz_t y, const bool neg) {
	mpz_t tempy;
	mpz_init(tempy);
	// y = x^3
	mpz_powm_ui(tempy, x, 3, p);
	// y = x^3 + Ax
	mpz_addmul(tempy, x, A);
	mpz_mod(tempy, tempy, p);
	// y = x^3 + Ax + B
	mpz_add(tempy, tempy, B);
	mpz_mod(tempy, tempy, p);
	// y = sqrt(x^3 + Ax + B)
	gf_sqrt(tempy, tempy);
	if (mpz_cmp_si(tempy, -1) == 0) {
		mpz_clear(tempy);
		throw std::runtime_error("Invalid point");
		std::terminate();
	}
	mpz_fdiv_q_2exp(y, p, 1);
	std::cout << "p/2: " << mpz_get_str(NULL, 10, y) << std::endl;
	if (neg) {
		mpz_sub(y, p, tempy);
	} else {
		mpz_set(y, tempy);
	}
	mpz_mod(y, y, p);
	mpz_clear(tempy);
}

EC_point::EC_point() {
	mpz_inits(x, y, NULL);
	identity = true;
}

EC_point::EC_point(const mpz_t x) {
	identity = false;
	mpz_inits(this->x, this->y, NULL);
	mpz_set(this->x, x);
	calc_y(this->x, y, false);
}

EC_point::EC_point(const double x) {
	identity = false;
	mpz_inits(this->x, this->y, NULL);
	mpz_set_d(this->x, x);
	calc_y(this->x, y, false);
}

EC_point::EC_point(const mpz_t x, const bool neg) {
	identity = false;
	mpz_inits(this->x, this->y, NULL);
	mpz_set(this->x, x);
	calc_y(this->x, y, neg);
}

EC_point::EC_point(const double x, const bool neg) {
	identity = false;
	mpz_inits(this->x, this->y, NULL);
	mpz_set_d(this->x, x);
	calc_y(this->x, y, neg);
}

EC_point::EC_point(const mpz_t x, const mpz_t y) {
	identity = false;
	mpz_inits(this->x, this->y, NULL);
	mpz_set(this->x, x);
	mpz_set(this->y, y);
}

EC_point EC_point::operator+(EC_point point) {
	// For some reason if this isn't here the program crashes
	if (mpz_cmp(x, point.x) < 0) {
		return point + *this;
	}
	// If the points are the same, return the double of the point
	if (mpz_cmp(x, point.x) == 0 && mpz_cmp(y, point.y) == 0) {
		return this->double_point();
	}
	// If the points are inverse, return the identity
	if (mpz_cmp(x, point.x) == 0) {
		return EC_point();
	}
	// If one of the points is the identity, return the other point
	if (identity) {
		return point;
	} else if (point.identity) {
		return *this;
	}
	mpz_t slope, temp, resx, resy;
	mpz_inits(slope, temp, resx, resy, NULL);
	// slope = (y2 - y1)
	mpz_sub(slope, y, point.y);
	mpz_mod(slope, slope, p);
	// temp = (x2 - x1)
	mpz_sub(temp, x, point.x);
	mpz_mod(temp, temp, p);
	// temp = temp^-1
	mpz_invert(temp, temp, p);
	// slope = slope * temp^-1
	mpz_mul(slope, slope, temp);
	mpz_mod(slope, slope, p);
	// resx = m^2
	mpz_powm_ui(resx, slope, 2, p);
	// resx = m^2 - x1
	mpz_sub(resx, resx, x);
	mpz_mod(resx, resx, p);
	// resx = m^2 - x1 - x2
	mpz_sub(resx, resx, point.x);
	mpz_mod(resx, resx, p);
	// y = resx - x1
	mpz_sub(resy, resx, x);
	mpz_mod(resy, resy, p);
	// y = m(resx - x1)
	mpz_mul(resy, resy, slope);
	mpz_mod(resy, resy, p);
	// y = y1 + m(resx - x1)
	mpz_add(resy, resy, y);
	mpz_mod(resy, resy, p);
	// reflect across x axis -(y + x)
	mpz_sub(resy, p, resy);
	mpz_mod(resy, resy, p);
	// Create the point
	EC_point result(resx, resy);
	// Clean up
	mpz_clears(slope, temp, resx, resy, NULL);
	return result;
}

void EC_point::operator+=(EC_point point) {
	EC_point result = *this + point;
	mpz_set(x, result.x);
	mpz_set(y, result.y);
}

EC_point EC_point::operator*(mpz_t n) {
	if (mpz_cmp_si(n, 0) == 0) {
		return EC_point(0.0);
	}
	if (mpz_cmp_si(n, 1) == 0) {
		return *this;
	}
	// Implement the double-and-add algorithm (based on square-multiply algorithm)
	EC_point result = EC_point(x, mpz_cmp(y, 0) < 0);
	for (int i = mpz_sizeinbase(n, 2) - 2; i >= 0; i--) {
		result = result.double_point();
		if (mpz_tstbit(n, i)) {
			result += EC_point(x, y);
		}
	}
	return result;
}

EC_point EC_point::double_point() {
	mpz_t slope, temp, resx, resy;
	mpz_inits(slope, temp, resx, resy, NULL);
	// slope = 3x^2
	mpz_powm_ui(slope, x, 2, p);
	mpz_mul_ui(slope, slope, 3);
	mpz_mod(slope, slope, p);
	// slope = 3x^2 + A
	mpz_add(slope, slope, A);
	mpz_mod(slope, slope, p);
	// temp = 2y
	mpz_mul_ui(temp, y, 2);
	mpz_mod(temp, temp, p);
	// temp = temp^-1
	mpz_invert(temp, temp, p);
	// slope = slope * temp^-1
	mpz_mul(slope, slope, temp);
	mpz_mod(slope, slope, p);
	// resx = m^2
	mpz_powm_ui(resx, slope, 2, p);
	// resx = m^2 - x1
	mpz_sub(resx, resx, x);
	mpz_mod(resx, resx, p);
	// resx = m^2 - 2x1
	mpz_sub(resx, resx, x);
	mpz_mod(resx, resx, p);
	// y = resx - x1
	mpz_sub(resy, resx, x);
	mpz_mod(resy, resy, p);
	// y = m(resx - x1)
	mpz_mul(resy, resy, slope);
	mpz_mod(resy, resy, p);
	// y = y1 + m(resx - x1)
	mpz_add(resy, resy, y);
	mpz_mod(resy, resy, p);
	// reflect across x axis -(y + x)
	mpz_sub(resy, p, resy);
	mpz_mod(resy, resy, p);
	// Create the point
	EC_point result(resx, resy);
	// Clean up
	mpz_clears(slope, temp, resx, resy, NULL);
	return result;
}

char *EC_point::print_point(FILE *out) {
	if (out != NULL) {
		if (identity) {
			fprintf(out, "O");
		} else {
			fprintf(out, "(%s, %s)", mpz_get_str(NULL, 10, x), mpz_get_str(NULL, 10, y));
		}
		return NULL;
	} else {
		char *result;
		if (identity) {
			result = (char *)malloc(sizeof(char) * 2);
			strncpy(result, "O", 2);
		} else {
			result = (char *)malloc(sizeof(char) * (mpz_sizeinbase(x, 10) + mpz_sizeinbase(y, 10) + 5));
			sprintf(result, "(%s, %s)", mpz_get_str(NULL, 10, x), mpz_get_str(NULL, 10, y));
		}
		return result;
	}
}

char *EC_point::print_point(FILE *out, int base) {
	if (out != NULL) {
		if (identity) {
			fprintf(out, "O");
		} else {
			fprintf(out, "(%s, %s)", mpz_get_str(NULL, base, x), mpz_get_str(NULL, base, y));
		}
		return NULL;
	} else {
		char *result;
		if (identity) {
			result = (char *)malloc(sizeof(char) * 2);
			strncpy(result, "O", 2);
		} else {
			result = (char *)malloc(sizeof(char) * (mpz_sizeinbase(x, base) + mpz_sizeinbase(y, base) + 5));
			sprintf(result, "(%s, %s)", mpz_get_str(NULL, base, x), mpz_get_str(NULL, base, y));
		}
		return result;
	}
}

// std::string hash(std::string message, std::string key) { return NULL; }
