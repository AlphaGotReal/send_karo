
class Solution:
	def rotate0(self, matrix):
		n = len(matrix)
		for y in range(n):
			for x in range(y):
				matrix[y][x], matrix[x][y] = matrix[x][y], matrix[y][x]

		for y in range(n):	
			for x in range(n//2):
				matrix[y][x], matrix[y][n-x-1] = matrix[y][n-x-1], matrix[y][x]

		return matrix

	def rotate(self, matrix):
		n = len(matrix)
		for y in range(n):
			for x in range(y):
				matrix[y][x], matrix[x][n-y-1] = matrix[x][n-y-1], matrix[y][x] 

		return matrix
		

s = Solution()

m = [
	[1, 2, 3],
	[4, 5, 6],
	[7, 8, 9]
]


def display(m):
	n = len(m)
	for y in range(n):
		for x in range(n):
			print(m[y][x], end='\t')
		print()

display(s.rotate(m))
 
