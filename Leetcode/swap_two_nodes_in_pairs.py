
class ListNode:
	def __init__(self, val=0, next=None):
		self.val = val
		self.next = None

class Solution:
	def swapPairs(self, head):
		if (head == None or head.next == None): return head
		curr = head
		while (curr):

			node1 = curr
			node2 = curr.next

			node2.next = node1
			node1.next = curr.next.next

			

		return head

def toList(head):
	l = []
	curr = head
	while curr:
		l += [curr.val]
		curr = curr.next
	return l

def toLink(l):
	head = ListNode(l[0])
	curr = head
	for r in l[1:]:
		curr.next = ListNode(r)
		curr = curr.next
	return head


sol = Solution()

l = [1, 2, 3, 4]
l = toList(sol.swapPairs(toLink(l)))
print(l)

