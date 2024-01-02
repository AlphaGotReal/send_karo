#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	bool increasingTriplet(vector<int> &nums){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		int n = nums.size();
		int streak = 1;
		int mx = nums[0];
		int mn = nums[0];
		for (int t = 1; t < n; ++t){
			if (mx < nums[t]){
				mx = nums[t];
				streak++;
			}if (mn > nums[t]){
				mn = nums[t];
				streak = 1;
			}if (streak == 3) return true;
		}return false;
	}
};


