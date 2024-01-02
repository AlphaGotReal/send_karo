
class Solution:
	def getRow(self, n):
		if n == 0:
			return [1]
		l = [1] + [0]*(n-1) + [1]
		for r in range(1, n//2+1):
			t = self.nCr(n, r)
			l[r] = t
			l[n-r] = t
		if (n%2 == 0):
			l[n//2] = self.nCr(n, n//2)
		return l

	def nCr(self, n, r):
		t = 1
		d = 1
		for a in range(n-r+1, n+1):
			t = t*a
			d = d*(n-a+1)
		
		return t//d

