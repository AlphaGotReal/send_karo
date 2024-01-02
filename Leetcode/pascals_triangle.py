
class Solution:
	def generate(self, n):
		if (n == 0): return []
		if (n == 1): return [[1]]
		ans = [[1], [1, 1]]
		for r in range(2, n):
			l = [1]
			for t in range(1, r):
				l.append(ans[r-1][t-1] + ans[r-1][t])
			l += [1]
			ans += [l]
		return ans

sol = Solution()
print(sol.generate(5))

