
class Solution{
	public static int[] twoSum(int[] nums, int target){
		int[] arr = {-1, -1};
		int[] hash = new int[1000];
		for (int t = 0; t < nums.length; t++){
			if (nums[t] > target){
				continue;
			}if (hash[target-nums[t]] > 0){
				arr[0] = hash[target-nums[t]]-1;
				arr[1] = t;
				return arr;
			}hash[nums[t]] = t + 1;
		}return arr;
	}

	public static void main(String[] args){
		int[] nums = {3, 4, 5, 6};
		int target = 6;
		int[] arr = Solution.twoSum(nums, target);
		System.out.print(arr[0]);
		System.out.print(", ");
		System.out.println(arr[1]);
	}
}


