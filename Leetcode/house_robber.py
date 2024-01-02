
class Solution:
	def rob(self, nums):
		if len(nums) == 0:
			return 0
		if len(nums) == 1:
			return nums[0]

		return max(nums[0] + self.rob(nums[2:]), self.rob(nums[1:]))

		

