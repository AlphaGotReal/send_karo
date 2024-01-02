
class remove_duplicates_from_sorted_array{
	public static void main(String[] args){
		int[] nums = {0, 0, 1, 1, 2, 3, 4};
		Solution s = new Solution();
		int some = s.removeDuplicates(nums);
	}
}

class Solution{
	public int removeDuplicates(int[] nums){
		int headstart = 0, r;
		for (r = 0; r < nums.length - 1; r++){
			if (nums[r] == nums[r+1]){
				headstart ++;
				continue;
			}nums[r - headstart] = nums[r];
		}nums[r - headstart] = nums[r];
		return nums.length - headstart;
	}
}

