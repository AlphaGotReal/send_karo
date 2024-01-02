
class Solution:
	def letterCombinations(self, digits):
		if digits == "":
			return []

		letters = [0, 96, 99, 102, 105, 108, 111, 115, 118, 122]



		first = ord(digits[0]) - 48
		if len(digits) == 1:
			return [chr(f+1) for f in range(letters[first-1], letters[first])]

		poss = []
		for f in range(letters[first-1], letters[first]):
			for l in self.letterCombinations(digits[1:]):
				poss += [chr(f+1) + l]

		return poss
	
		 
		
			


