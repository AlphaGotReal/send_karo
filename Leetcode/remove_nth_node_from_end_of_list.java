
class ListNode{
	int val;
	ListNode next;

	ListNode(){}
	ListNode(int val){
		this.val = val;
	}ListNode(int val, ListNode next){
		this.val = val;
		this.next = next;
	}

	public int[] toArr(){
		int[] arr = new int[10];
		int t = 0;
		ListNode curr = this;
		while (curr != null){
			arr[t] = curr.val;
			t++;
			curr = curr.next;
		}return arr;
	}
	public void store(int[] arr){
		ListNode curr = this;
		for (int r = 0; r < arr.length; r++){
			curr.val = arr[r];
			curr.next = new ListNode();
			curr = curr.next;
		}
	}
}

class Solution{
	public ListNode removeNthFromEnd(ListNode head, int n){
		int total = 0;
		ListNode curr = head;
		while (curr != null){
			curr = curr.next;
			total++;
		}if (total == n){
			return head.next;
		}curr = head;
		for (int i = 0; i < total-n-1; i++){
			curr = curr.next;
			if (curr == null) return head;
		}curr.next = curr.next.next;
		return head;
	}
}

class run{
	public static void main(String[] a){
		Solution sol = new Solution();
		ListNode head = new ListNode();
		head.val = 1;
		head.next = new ListNode(2);
		head.next.next = new ListNode(3);
		head.next.next.next = new ListNode(4);
		head.next.next.next.next = new ListNode(5);
		head = sol.removeNthFromEnd(head, 1);
		
		ListNode curr = head;
		while (curr != null){
			System.out.println(curr.val);
			curr = curr.next;
		}
	}

	
}

