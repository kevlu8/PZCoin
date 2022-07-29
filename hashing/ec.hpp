#include "include.hpp"

typedef struct EC_point {
	mpz_t x;
	mpz_t y;
} EC_point;

/**
 * @brief Computes the sum of two points on an elliptic curve
 *
 * @param p1 First point
 * @param p2 Second point
 * @return Sum of the two points
 */
EC_point add_points(EC_point p1, EC_point p2);

/**
 * @brief Multiplies a point on an elliptic curve by a scalar
 *
 * @param p Point to multiply
 * @param n Scalar to multiply by
 * @return Result of the multiplication
 */
EC_point multiply_point(EC_point p, mpz_t n);
