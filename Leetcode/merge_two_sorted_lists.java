
class ListNode{
	int val;
	ListNode next;

	ListNode(){}
	ListNode(int val){
		this.val = val;
	}
	ListNode(int val, ListNode next){
		this.val = val;
		this.next = next;
	}

	void display(){
		ListNode curr = this;
		while (curr != null){
			System.out.println(curr.val);
			curr = curr.next;
		}
	}

	void append(int data){
		ListNode curr = this;
		while (curr.next != null){
			curr = curr.next;
		}curr.next = new ListNode(data);
	}

	static ListNode intToNode(int[] arr){
		ListNode node = new ListNode(arr[0]);
		for (int index = 1; index < arr.length; index ++){
			node.append(arr[index]);
		}return node;
	}
}

class Solution{
	public static ListNode mergeTwoLists0(ListNode l1, ListNode l2){
		ListNode node = new ListNode();
		ListNode curr = node;
		while (l1 != null || l2 != null){
			if (l1 == null){
				curr.next = new ListNode(l2.val);
				l2 = l2.next;
				curr = curr.next;
				continue;
			}if (l2 == null){
				curr.next = new ListNode(l1.val);
				l1 = l1.next;
				curr = curr.next;
				continue;
			}if (l1.val >= l2.val){
				curr.next = new ListNode(l2.val);
				l2 = l2.next;
				curr = curr.next;
				continue;
			}curr.next = new ListNode(l1.val);
			l1 = l1.next;
			curr = curr.next;
		}return node.next;
	}

	public static ListNode mergeTwoLists(ListNode l1, ListNode l2){
		ListNode buffer;
		if (l1 == null) return l1;
		if (l2 == null) return l2;
		ListNode p1 = l1;
		ListNode p2 = l2;
		if (l2.val > l1.val){
			p1 = l2;
			p2 = l1;
		}
		while (p1.next.val <= p2.val){
			p1 = p1.next;
		}buffer = p1.next;
		p1.next = p2;
		p2 = p2.next;
		p1 = p1.next;
		p1.next = Solution.mergeTwoLists(buffer, p2);
		return p1;
	}

	void append(ListNode head, int data){
		ListNode curr = head;
		while (curr.next != null){
			curr = curr.next;
		}curr.next = new ListNode(data);
	}

	public static void main(String[] args){
		//System.out.println("hello world");
		int[] arr1 = {1, 2, 3};
		int[] arr2 = {2, 5, 6};
		ListNode l1 = ListNode.intToNode(arr1);
		ListNode l2 = ListNode.intToNode(arr2);
		Solution.mergeTwoLists(l1, l2).display();
	}
}

