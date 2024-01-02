#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	int longestConsecutive(vector<int> &arr){
		ios_base::sync_with_stdio(0);
		cin.tie(0);

		if (arr.size() == 0){
			return 0;
		}

		sort(arr.begin(), arr.end());
		int count = 0;
		int best = 0;

		for (int r = 1; r < arr.size(); ++r){
			if (arr[r] == arr[r-1]) continue;
			if (arr[r] == arr[r-1] + 1){
				++count;
				continue;
			}best = max(best, count);
			count = 0;
		}return best+1;
	}
};


