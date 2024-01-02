
class Treenode{
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

class Solution{
	public int maxDepth(TreeNode root){
		if (root == null) return 0;
		return 1 + this.max(this.maxDepth(root.left), this.maxDepth(root.right))
	}public int max(int a, int b){
		if (a > b) return a;
		return b;
	}
}


