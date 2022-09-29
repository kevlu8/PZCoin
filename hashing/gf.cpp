#include "gf.hpp"

mpz_t p, f;

void gf_init() {
	mpz_init_set_ui(f, 1);
	mpz_mul_2exp(f, f, 571);
	mpz_init_set(p, f);
	mpz_add_ui(p, p, 0x425);
}

void gf_mul(mpz_t ans, mpz_t a, mpz_t b) {
	mpz_t res, tempa, tempb;
	mpz_init_set_ui(res, 0);
	mpz_init_set(tempa, a);
	mpz_init_set(tempb, b);
	for (int i = 0; i < mpz_sizeinbase(tempb, 2); i++) {
		if (mpz_tstbit(tempb, i))
			gf_add(res, res, tempa);
		mpz_mul_2exp(tempa, tempa, 1);
		if (mpz_cmpabs(res, p) >= 0)
			gf_sub(res, res, p);
	}
	gf_mod(ans, res);
	mpz_clear(res);
}

void gf_div(mpz_t ans, mpz_t a, mpz_t b) {
	mpz_t inv;
	mpz_init(inv);
	gf_inv(inv, b);
	gf_mul(ans, a, inv);
	mpz_clear(inv);
}

void gf_pow(mpz_t ans, mpz_t a, mpz_t b) {
	mpz_t res, tempa, tempb;
	mpz_init_set_ui(res, 1);
	mpz_init_set(tempa, a);
	mpz_init_set(tempb, b);
	if ((mpz_cmp_ui(a, 0) | mpz_cmp_ui(b, 0)) == 0) {
		throw std::runtime_error("0^0 is undefined");
		std::terminate();
	}
	for (int i = 0; i < mpz_sizeinbase(tempb, 2); i++) {
		if (mpz_tstbit(tempb, i))
			gf_mul(res, res, tempa);
		gf_mul(tempa, tempa, tempa);
	}
	gf_mod(ans, res);
	mpz_clear(res);
}

void gf_inv(mpz_t ans, mpz_t a) {
	mpz_t b;
	mpz_init(b);
	mpz_sub_ui(b, f, 2);
	gf_pow(ans, a, b);
	mpz_clear(b);
}

void gf_mod(mpz_t ans, mpz_t a) {
	mpz_t res, tmp;
	mpz_init_set(res, a);
	mpz_init(tmp);
	// std::cout << "a = " << mpz_get_str(NULL, 10, a) << std::endl;
	while (mpz_cmpabs(res, p) >= 0) {
		mpz_mul_2exp(tmp, p, mpz_sizeinbase(res, 2) - mpz_sizeinbase(p, 2));
		mpz_xor(res, res, tmp);
	}
	mpz_set(ans, res);
	mpz_clear(res);
}

void gf_sqrt(mpz_t ans, mpz_t a) {
	mpz_t tmp;
	mpz_init(tmp);
	mpz_div_2exp(tmp, f, 1);
	gf_pow(ans, a, tmp);
}
