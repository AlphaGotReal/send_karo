
class Solution:
	def areNumbersAscending(self, s):
		last = -1
		this = 0
		s = s + ' '
		for r in range(len(s)-1):
			if (48 <= ord(s[r]) <= 57):
				if (48 <= ord(s[r+1]) <= 57):
					this = this*10 + (ord(s[r]) - 48)*10
					continue;
				this += ord(s[r]) - 48
				if (this < last):
					return False
				last = this
				this = 0

		return True
				

