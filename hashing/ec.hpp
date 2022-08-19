#include "include.hpp"

// Must be called before any other function.
void EC_init();

class EC_point {
public:
	bool identity;
	mpz_t x;
	mpz_t y;
	EC_point();
	EC_point(const mpz_t);
	EC_point(const double);
	EC_point(const mpz_t, const bool);
	EC_point(const double, const bool);
	EC_point(const mpz_t, const mpz_t);
	EC_point operator+(EC_point);
	void operator+=(EC_point);
	void operator-();
	EC_point operator*(mpz_t);
	/**
	 * @brief Double a point on an elliptic curve
	 *
	 * @return Result of the doubling
	 */
	EC_point double_point();
	/**
	 * @brief Print the point to the given output stream
	 *
	 * @param out Output stream to print to
	 */
	char *print_point(FILE *);
	/**
	 * @brief Print the point to the given output stream
	 *
	 * @param out Output stream to print to
	 * @param base Base to print in
	 */
	char *print_point(FILE *, int);
};

/**
 * @brief Hash the message with the given key
 *
 * @param message Message to hash
 * @param key Key to hash with
 * @return Hash of the message
 */
// std::string hash(std::string, std::string);
