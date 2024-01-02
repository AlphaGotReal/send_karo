
class Solution:
	#brute force
	def generateParenthesis0(self, n):
		n = 2*n
		switch = {'0': '(', '1': ')'}
		all = []

		def isvalid(string):
			if string[0] == ')':
				return False
			op = [string[0]]
			for r in string[1:]:
				if r == '(':
					op += [r]
					continue
				if op:
					op.pop()
					continue
				return False
			return op == []

		for r in range(2**n):
			number = '0'*(n-len(bin(r)[2:])) + bin(r)[2:]
			st = ''
			for c in number:
				st += switch[c]
			if isvalid(st):
				all += [st]

		return all


	def generateParenthesis(self, n):
		all = []
		


s = Solution()

