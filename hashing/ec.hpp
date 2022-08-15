#include "include.hpp"

#define B 8

class EC_point {
public:
	mpq_t x;
	mpq_t y;
	EC_point();
	EC_point(const mpq_t);
	EC_point(const double);
	EC_point(const mpq_t, const bool);
	EC_point(const double, const bool);
	EC_point(const mpq_t, const mpq_t);
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
