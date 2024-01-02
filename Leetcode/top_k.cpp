#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<int> topKFrequent(vector<int> &arr, int k){
		sort(arr.begin(), arr.end());
		
	}
};

int main(){
	Solution sol;
	vector<int> arr = {1, 2, 2};
	vector<int> out = sol.topKFrequent(arr, 2);
	for (int t : out){
		cout << t << endl;
	}

	return 0;
}


