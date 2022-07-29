# PZHash
- This hashing function uses 256-bit ECDSA with Curve25519 to sign a nonce using a public key included in the block
- The public key is a randomly generated 64 bit integer that must be unique for the 1000 latest blocks

# Techinical stuff
- The equation of the curve being used will be $y^{2} = x^{3} + 486662x^{2} + x$
  - This will actually be expressed as $y = \pm \sqrt{x^{3} + 486662x^{2} + x}$
- The derivative of this equation is $y' = \frac{3x^{2} + 973324x + 1}{2\sqrt{x^{3} + 486662x^{2} + x}}$
- Base is x = 9
- Everything is modulo $2^{255} - 19$
