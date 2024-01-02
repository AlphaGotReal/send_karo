#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<vector<int>> threeSum(vector<int> &arr){
		ios_base::sync_with_stdio(0);
		cin.tie(0);

		sort(arr.begin(), arr.end());
		int n = arr.size();

		set<vector<int>> s;
		for (int t = 0; t < n; ++t){
			int r = n-1;
			int l = t+1;
			int sum;
			int target = -arr[t];
			while (r > l){
				sum = arr[r] + arr[l];
				if (target == sum){
					s.insert({arr[t], arr[l], arr[r]});
				}if (sum >= target) --r;
				else ++l;
			}
		}

		vector<vector<int>> ans;
		for (auto out: s){
			ans.push_back(out);
		}
		return ans;
	}
};

int main(){

	vector<int> arr = {-1, 0, 1, 2, -1, -4};

	Solution sol;
	vector<vector<int>> ans = sol.threeSum(arr);

	for (vector<int> out: ans){
		cout << out[0] << ' ' << out[1] << ' ' << out[2] << endl;
	}
	return 0;
}


