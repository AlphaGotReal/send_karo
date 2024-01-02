
class ListNode:
	def __init__(self, val=0, next=None):
		self.val = val
		self.next = next

class Solution:
	def deleteDuplicates(self, head):
		if head is None:
			return head
		curr = head
		while curr.next:
			while (curr.val == curr.next.val):
				curr.next = curr.next.next
				if not curr:
					return head

			if curr:
				curr = curr.next
				continue
			return head
		return head

def toNode(l):
	head = ListNode(l[0])
	curr = head
	for r in l[1:]:
		curr.next = ListNode(r)
		curr = curr.next
	return head

def toList(h):
	l = []
	curr = h
	while curr:
		l += [curr.val]
		curr = curr.next
	return l 


