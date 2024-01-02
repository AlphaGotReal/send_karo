
class Solution:
	def search(self, nums, target):
		n = len(nums)
		first = target > nums[n-1]
		
	def findp(self, nums, i):
		if nums[0] > nums[i] and nums[0] <= nums[i-1]:
			return i
		return self.findp(nums, )
