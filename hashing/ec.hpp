#include "include.hpp"

class EC_point {
public:
	mpz_t x, y;
	EC_point();
	EC_point(const mpz_t, const mpz_t);
	EC_point operator+(EC_point);
	void operator+=(EC_point);
	EC_point operator*(mpz_t);
	/**
	 * @brief Doubles a point on an elliptic curve
	 * 
	 * @return Result of the doubling
	 */
	EC_point double_point();
};
