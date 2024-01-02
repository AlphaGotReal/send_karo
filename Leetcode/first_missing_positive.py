
class Solution:
	def firstMissingPositive0(self, nums):
		nums.sort()
		first = 1
		for r in nums:
			if r == last:
				first += 1

		return last


