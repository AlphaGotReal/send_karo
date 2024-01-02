class Solution:
	def lengthOfLastWord(self, s):
		count = 0
		for r in range(len(s)-1,-1,-1):
			if s[r] != ' ':
				count += 1
			if count != 0 and s[r] == ' ':
				return count


