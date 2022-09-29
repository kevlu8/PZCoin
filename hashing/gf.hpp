#include "include.hpp"
#define gf_add(ans, a, b) mpz_xor(ans, a, b);
#define gf_sub(ans, a, b) mpz_xor(ans, a, b);

// Must be called before any other function.
void gf_init();

void gf_mul(mpz_t, mpz_t, mpz_t);
void gf_div(mpz_t, mpz_t, mpz_t);
void gf_pow(mpz_t, mpz_t, mpz_t);
void gf_inv(mpz_t, mpz_t);
void gf_mod(mpz_t, mpz_t);
void gf_sqrt(mpz_t, mpz_t);
