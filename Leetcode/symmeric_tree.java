
class TreeNode{
	int val;
	TreeNode left;
	TreeNode right;
	TreeNode(){}
	TreeNode(int val){
		this.val = val;
	}TreeNode(int val, TreeNode left, TreeNode right){
		this.val = val;
		this.left = left;
		this.right = right;
	}
}

class Solution(){
	public boolean isSymmetric(TreeNode root){
		if (root.val == null) return true;
		
		TreeNode L = root.left;
		TreeNode R = root.right;

		return compare(L, R);		

	}
	public boolean compare(TreeNode node1, TreeNode node2){
		if (node1.val != node2.val) return false;
		if (node1.right == null && node2.left == null) return true;
		if (node1.right == null || node2.right == null) return false;
		if (node1.left.val != node2.right.val || node1.right.val != node2.left.val) return false;
		return (this.compare(node1.right, node2.left) && this.compare(node1.left, node2.right));
	}
}

