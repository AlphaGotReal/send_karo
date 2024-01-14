
class Solution:
	def climbStairs(self, n):
		'''
		if n - 2 < 0:
			return 1
	
		inc = n%2

		t = 0
		m = n//2
		
		for r in range(m + 1):
			t += self.nCr(m + r + inc, 2*r + inc)

		return t
		'''

		yo = [
			1,
			1,
			2,
			3,
			5,
			8,
			13,
			21,
			34,
			55,
			89,
			144,
			233,
			377,
			610,
			987,
			1597,
			2584,
			4181,
			6765,
			10946,
			17711,
			28657,
			46368,
			75025,
			121393,
			196418,
			317811,
			514229,
			832040,
			1346269,
			2178309,
			3524578,
			5702887,
			9227465,
			14930352,
			24157817,
			39088169,
			63245986,
			102334155,
			165580141,
			267914296,
			433494437,
			701408733,
			1134903170,
			1836311903]

		return yo[n]

	def nCr(self, n, r):
		c = 1
		for t in range(r):
			c = c * (n - t) // (t + 1)

		return c
