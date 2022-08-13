# PZHash
- This hashing function uses a ECOH-512 hash
- It hashes the concatenation of the current block and the hash of the previous block

# Technical stuff
- The equation of the curve being used will be $y^2 + xy = x^3 + b$ where b is a very large number
	- This will actually be expressed as $y = \pm \sqrt{x^3 + \frac{1}{4}x^2 + b} - \frac{1}{2}x$
- The derivative of this equation is $y' = \pm \frac{3x^2 + \frac{1}{2}x}{2\sqrt{x^3 + \frac{1}{4}x^2 + b}} - \frac{1}{2}$
- Base point is a very large number that is specified in the curve standard
- Everything is modulo $x^{571} + x^{10} + x^{5} + x^{2} + 1$
- To get point of intersection with curve: $x_3 = m^2 + m-x_1 - x_2$ or $x_2 = m^2 + m - 2x_1$
	- Derivation of the equations below

1. Set the curve ( $y^2 = x^3 + Ax^2 + x$ ) and the tangent or secant line ( $y = mx + b$ ) equal to each other
	1. $x^3 + Ax^2 + x = (mx + b)^2$
	2. $x^3 + Ax^2 + x = m^2x^2 + 2mbx + b^2$
2. Move them to the other side of the equation and combine like terms
	1. $x^3 + Ax^2 + x - m^2x^2 - 2mbx - b^2 = 0$
	2. $x^3 + (A - m^2)x^2 + (1 - 2mb)x - b^2 = 0$
3. Express the points of intersection as a factored cubic equation
	1. $(x - x_1)(x - x_2)(x - x_3) = 0$
4. Expand the equation and collect like terms
	1. $(x - x_1)(x^2 - x_2x - x_3x + x_2x_3) = 0$
	2. $x^3 - x_2x^2 - x_3x^2 + x_2x_3x - x_1x^2 + x_1x_2x + x_1x_3x - x_1x_2x_3 = 0$
	3. $x^3 + (-x_1 - x_2 - x_3)x^2 + (x_1x_2 + x_1x_3 + x_2x_3)x - x_1x_2x_3 = 0$
5. Take the coefficients of the 2nd degree on both equations and set them equal to each other
	1. $-x_1 - x_2 - x_3 = A - m^2$
6. Isolate $x_3$
	1. $-x_2 - x_3 = A - m^2 + x_1$
	2. $-x_3 = A - m^2 + x_1 + x_2$
	3. $x_3 = m^2 - A - x_1 - x_2$
7. For the other equation, $x_1$ and $x_2$ are equal as the two roots overlap each other
	1. $x_3 = m^2 - A - 2x_1$
	2. $x_2 = m^2 - A - 2x_1$
