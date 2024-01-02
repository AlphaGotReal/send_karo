

class Solution:
	def addBinary(self, a, b):
		sum = ''
		carry = '0'
		ia = len(a) - 1
		ib = len(b) - 1

		while (ib >= 0 or ia >= 0):
			if (ia >= 0 and ib >= 0):
				r, u = self.add(a[ia], b[ib])
				carry, u = self.add(u, carry)
				if r == '1':
					carry = '1'
			elif ia < 0 :
				carry, u = self.add(carry, b[ib])

			elif ib < 0:
				carry, u = self.add(carry, a[ia])
			sum = u + sum
			ib = ib - 1
			ia = ia - 1

		if carry == '1':
			return carry + sum
		return sum

	def add(self, x, y):
		if x == '1' and y == '1':
			return '1' , '0'
		elif x == '0' and y == '0':
			return '0' , '0'

		return '0', '1'

s = Solution()




