#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<bool> kidsWithCandies(vector<int> &candies, int extraCandies){
		ios_base::sync_with_stdio(0);
		cin.tie(0);

		vector<bool> out(candies.size());
		int mx = candies[0];

		for (int t: candies){
			mx = max(mx, t);
		}

		for (int t = 0; t < candies.size(); ++t){
			out[t] = (candies[t] + extraCandies >= mx); 
		}return out;
	}
};


