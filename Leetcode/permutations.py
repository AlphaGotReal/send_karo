
class Solution:
	def permute(self, nums):
		l = []
		
		if len(nums) == 2:
			return [
				[nums[0], nums[1]],
				[nums[1], nums[0]]
			]

		for r in range(len(nums)):
			w = nums.copy()
			e = w.pop(r)
			l += [[e] + t for t in self.permute(w)]

		return l
			

