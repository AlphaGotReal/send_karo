
class Solution:
	def longestValidParentheses(self, s):
		if not s:
			return 0
		if isValid(s):
			return len(s)
		return max(self.longestValidParentheses(s[1:]), self.longestValidParentheses(s[:-1]))

	def isValid(self, s):
		if s[0] == ')': 
			return False
		op = []
		for r in s:
			if r == '(':
				op += [r]
				continue
			if op:
				op.pop()
				continue
			return False
		return op == []



