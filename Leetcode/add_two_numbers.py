
class ListNode:
	def __init__(self, val):
		self.val = val
		self.next = None

class LinkedList:
	def __init__(self, head = None):
		self.head = head
	
	def append(self, data):
		curr = self.head
		while (curr != None):
			if (curr.next == None):
				break
			curr = curr.next
		curr.next = ListNode(data)

	def toList(self):
		l = []
		curr = self.head
		while (curr != None):
			l += [curr.val]
			curr = curr.next
		return l

class Solution:

	def addTwoNumbers0(self, l1, l2):
		carry = 0
		
		c1 = l1
		s1 = 0

		c2 = l2
		s2 = 0

		while (c1 != None or c2 != None):
			
			s1 = 0 
			s2 = 0 
			
			if c1 is not None:
				s1 = c1.val

			if c2 is not None:
				s2 = c2.val

			s = s1 + s2 + carry
			carry = s//10

			s = s%10

			if c1 is not None: 
				c1.val = s
				c1 = c1.next
				l = l1

			if c2 is not None:
				c2.val = s
				c2 = c2.next
				l = l2

		if carry:
			curr = l
			while (curr.next != None):
				curr = curr.next
			curr.next = ListNode(1)

		return l

	def addTwoNumbers(self, l1, l2):
		if l1 is None and l2 is None:
			return None

		if (l1 is None):
			l1 = ListNode(0)

		if (l2 is None):
			l2 = ListNode(0)

		s = l1.val + l2.val

		rnode = ListNode(s%10)

		if s >= 10:
			if (l1.next is None):
				l1.next = ListNode(1)
			else:
				l1.next.val += 1

		rnode.next = self.addTwoNumbers(l1.next, l2.next)

		return rnode


def toLinkL(l):
	head = ListNode(l.pop(0))
	ll = LinkedList(head)
	while (l):
		ll.append(l.pop(0))

	return ll.head

l1 = toLinkL([9, 9, 9])
l2 = toLinkL([1])

s = Solution()



