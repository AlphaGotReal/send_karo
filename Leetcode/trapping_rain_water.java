
class Solution{
	public int trap(int[] height){
		int n = height.length;
		
		int rscan[] = new int[n];
		int lscan[] = new int[n];

		rscan[n-1] = height[n-1];
		lscan[0] = height[0];

		for (int r = 1; r < n; r++){
			if (rscan[n-r] < height[n-r]){
				rscan[n-r-1] = height[n-r-1];
			}else {
				rscan[n-r-1] = rscan[n-r];
			}

			if (lscan[r-1] < height[r]){
				lscan[r] = height[r];
			}else{
				lscan[r] = lscan[r-1];
			}
		}

		for (int r = 0; r < n; r++){
			System.out.print(lscan[r]);
			System.out.print(" ");
			System.out.println(rscan[r]);
		}

		int water = 0;
		for (int r = 0; r < n; r++){
			if (rscan[r] > lscan[r]){
				water += lscan[r] - height[r];
				continue;
			}water += rscan[r] - height[r];
		}return water;
	}
}

class Run{
	public static void main(String[] args){
		int height[] = {4, 2, 0, 3, 2, 5};
		Solution s = new Solution();
		int total = s.trap(height);

		System.out.println(total);
		
	}
}

