
class Solution:
	def groupAnagrams(self, strs):
		pass

	def isAnagram(self, str1, str2):
		if len(str1) != len(str2): return False

		counter = [0 for r in range(200)]
		
		for r in range(len(str1)):
			counter[ord(str1[r])] += 1
			counter[ord(str2[r])] -= 1

		for r in range(200):
			if counter[r] != 0:
				return False

		return True

