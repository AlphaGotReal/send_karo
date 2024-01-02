class Solution:
	def plusOne(self, digits):
		if digits[0] == 9:
			digits = [0] + digits
		n = len(digits)
		digits[n-1] += 1
		extra = 0
		while (n):
			n += -1
			digits[n] += extra
			extra = digits[n]//10
			digits[n] = digits[n]%10
				
		return digits
