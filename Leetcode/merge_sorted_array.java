
class Solution{
	public void merge(int[] nums1, int m, int[] nums2, int n){
		if (n == 0) return;
		if (m == 0){
			for (int r = 0; r < n; r++){
				nums1[r] = nums2[r];
			}return ;
		}for (int r = 0, t = 0; r < nums1.length; r++){
			if ((nums1[r] > nums2[t]) || (r >= m + t)){
				for(int w = nums1.length - 1; w > r; w--){
					nums1[w] = nums1[w-1];
				}nums1[r] = nums2[t++];
			}
		}
	}
}

class Run{
	public static void main(String[] args){
		Solution sol = new Solution();
		int[] nums1 = {0};
		int[] nums2 = {1};
		sol.merge(nums1, 0, nums2, 1);
		for (int r = 0; r < nums1.length; r++){
			System.out.println(nums1[r]);
		}
	}
}

