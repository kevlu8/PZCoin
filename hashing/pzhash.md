# PZHash
- This hashing function uses a ECOH-512 hash
- It hashes the concatenation of the current block and the hash of the previous block

# Technical stuff
- The equation of the curve being used will be $y^2 + xy = x^3 + B$ where B is a very large number
	- This will actually be expressed as $y = \pm \sqrt{x^3 + \frac{1}{4}x^2 + B} - \frac{1}{2}x$
- The derivative of this equation is $y' = \pm \frac{3x^2 + \frac{1}{2}x}{2\sqrt{x^3 + \frac{1}{4}x^2 + B}} - \frac{1}{2}$
- The base point and B are very large constants that is specified in the [curve standard](https://neuromancer.sk/std/nist/B-571)
- Everything is modulo $x^{571} + x^{10} + x^{5} + x^{2} + 1$
- To get point of intersection with curve: $x_3 = m^2 + m-x_1 - x_2$ or $x_2 = m^2 + m - 2x_1$
	- Derivation of the equations below

1. Set the y= form of the curve ($y = \pm \sqrt{x^3 + \frac{1}{4}x^2 + B} - \frac{1}{2}x$) and the tangent/secant line ($y = mx + b$) equal to each other
	1. $\pm \sqrt{x^3 + \frac{1}{4}x^2 + B} - \frac{1}{2}x = mx + b$
2. Get rid of the square root by moving $\frac{1}{2}x$ to the other side and squaring both sides
	1. $\pm \sqrt{x^3 + \frac{1}{4}x^2 + B} = mx + b + \frac{1}{2}x$
	2. $x^3 + \frac{1}{4}x^2 + B = m^2x^2 + mx^2 + \frac{1}{4}x^2 + 2mbx + bx + b^2$
3. Convert the equation into a cubic equation in =0 form by moving all terms to one side of the equals sign and combining like terms
	1. $m^2x^2 + mx^2 + \frac{1}{4}x^2 + 2mbx + bx + b^2 - x^3 - \frac{1}{4}x^2 - B = 0$
	2. $-x^3 + (m^2 + m)x^2 + (2mb + b)x + b^2 - B = 0$
	3. $x^3 - (m^2 + m)x^2 - (2mb + b)x - b^2 + B = 0$
4. Express the points of intersection as a factored cubic equation
	1. $(x - x_1)(x - x_2)(x - x_3) = 0$
5. Expand the equation and collect like terms
	1. $(x - x_1)(x^2 - x_2x - x_3x + x_2x_3) = 0$
	2. $x^3 - x_2x^2 - x_3x^2 + x_2x_3x - x_1x^2 + x_1x_2x + x_1x_3x - x_1x_2x_3 = 0$
	3. $x^3 - (x_1 + x_2 + x_3)x^2 + (x_1x_2 + x_1x_3 + x_2x_3)x - x_1x_2x_3 = 0$
6. Take the coefficients of the 2nd degree on both equations and set them equal to each other
	1. $x_1 + x_2 + x_3 = m^2 + m$
7. Isolate $x_3$
	1. $x_3 = m^2 + m - x_1 - x_2$
8. For the other equation, there are 2 points of intersection that are the same, so we can set $x_1$ equal to $x_2$ and simplify the previous equation
	1. $x_3 = m^2 + m - x_1 - x_1$
	2. $x_2 = m^2 + m - 2x_1$
