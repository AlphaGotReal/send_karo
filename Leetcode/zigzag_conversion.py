
class Solution:
	def convert(self, s, n):
		if n == 1:
			return s
		x = 0
		inc = 1
		l = [0]*(n+1)

		new = s

		for r in range(len(s)):
			l[x+1] += 1 
			x += inc
			if x in (0, n-1): inc = -inc

		for t in range(1, n+1):
			l[t] += l[t-1]

		x = 0
		inc = 1

		for r in range(len(s)):
			new = new[:l[x]] + s[r] + new[l[x]+1:]
			l[x] += 1
			x += inc
			if x in (0, n-1): inc = -inc

		return new
