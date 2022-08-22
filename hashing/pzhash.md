# PZHash
- This hashing function uses a ECOH-512 hash
- It hashes the concatenation of the current block, a nonce, and the hash of the previous block

# Technical stuff
- The equation of the curve being used will be $y^2 + xy = x^3 + B$ where B is a very large number
	- By transforming the elliptic curve from generic to simplified Weierstrauss form, we get $y^2 = x^3 + Ax + B - \frac{1}{216}$ where A is $\frac{1}{16}$
	- The transformation was done following [this](https://trustica.cz/en/2018/02/22/introduction-to-elliptic-curves/) article
- The derivative of this equation (calculated using implicit derivation) is $y' = \frac{3x^2 + A}{2y}$
	- The version of the equation that we use for ease of implementation is $y' = \frac{48x^2 + 16A}{32y}$
- The generator point and B are very large integer constants that are specified in the [curve standard](https://neuromancer.sk/std/nist/B-571)
- Everything is modulo $x^{571} + x^{10} + x^{5} + x^{2} + 1$
- To get point of intersection with curve: $x_3 = m^2 - x_1 - x_2$ or $x_2 = m^2 - 2x_1$
	- Derivation of the equations below

1. Set the curve ( $y^2 = x^3 + Ax + B - \frac{1}{216}$ ) and the tangent or secant line ( $y = mx + b$ ) equal to each other
	1. $x^3 + Ax + B - \frac{1}{216} = (mx + b)^2$
	2. $x^3 + Ax + B - \frac{1}{216} = m^2x^2 + 2mbx + b^2$
2. Move them to the other side of the equation and combine like terms
	1. $x^3 + Ax + B - \frac{1}{216} - m^2x^2 - 2mbx - b^2 = 0$
	2. $x^3 + m^2x^2 + (A + 2mb)x - b^2 + B - \frac{1}{216}$
3. Express the points of intersection as a factored cubic equation
	1. $(x - x_1)(x - x_2)(x - x_3) = 0$
4. Expand the equation and collect like terms
	1. $(x - x_1)(x^2 - x_2x - x_3x + x_2x_3) = 0$
	2. $x^3 - x_2x^2 - x_3x^2 + x_2x_3x - x_1x^2 + x_1x_2x + x_1x_3x - x_1x_2x_3 = 0$
	3. $x^3 + (-x_1 - x_2 - x_3)x^2 + (x_1x_2 + x_1x_3 + x_2x_3)x - x_1x_2x_3 = 0$
5. Take the coefficients of the 2nd degree on the 2 equations and set them equal to each other
	1. $-x_1 - x_2 - x_3 = m^2$
6. Isolate $x_3$
	1. $-x_2 - x_3 = m^2 + x_1$
	2. $-x_3 = A - m^2 + x_1 + x_2$
	3. $x_3 = m^2 - x_1 - x_2$
7. For the other equation, $x_1$ and $x_2$ are equal as the two roots overlap each other
	1. $x_3 = m^2 - 2x_1$
	2. $x_2 = m^2 - 2x_1$
