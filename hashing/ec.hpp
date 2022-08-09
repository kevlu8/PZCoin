#include "include.hpp"

#define A 8

class EC_point {
public:
	mpz_t x, y;
	EC_point();
	EC_point(const mpz_t, const mpz_t);
	EC_point(const int, const int);
	EC_point(const mpz_t);
	EC_point(const int);
	~EC_point();
	EC_point operator+(EC_point);
	void operator+=(EC_point);
	EC_point operator*(mpz_t);
	/**
	 * @brief Double a point on an elliptic curve
	 *
	 * @return Result of the doubling
	 */
	EC_point double_point();
};

/**
 * @brief Hash the message with the given key
 *
 * @param message Message to hash
 * @param key Key to hash with
 * @return Hash of the message
 */
std::string hash(std::string, std::string);
