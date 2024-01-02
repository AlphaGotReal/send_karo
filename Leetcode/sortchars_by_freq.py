class Solution:
	def frequencySort0(self, s):
		hmap = [0 for r in range(200)]
		index = [r for r in range(200)]
		for char in s:
			hmap[ord(char)] += 1
		
		newhmap = []
		newindex = []

		for t in range(200):
			if hmap[t] != 0:
				newhmap += [hmap[t]]
				newindex += [t]

		for y in range(len(newhmap) - 1):
			for x in range(len(newhmap) - y - 1):
				if newhmap[x] > newhmap[x+1]:
					newhmap[x], newhmap[x+1] = newhmap[x+1], newhmap[x]
					newindex[x], newindex[x+1] = newindex[x+1], newindex[x]

		final = ''
		for r in range(len(newhmap)):
			for t in range(newhmap[r]):
				final = chr(newindex[r]) + final

		return final
				

	def frequencySort(self, s):
		pass

