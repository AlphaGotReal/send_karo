#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<int> grayCode(int n){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		if (n == 0) return {0};
		vector<int> out;
		int pow_two = pow(2, n), m;
		for (int t = 0; t < pow_two; ++t){
			m = t ^ (t >> 1);
			out.push_back(m);
		}return out;
	}
};


int main(){
	Solution sol;

	int n; cin >> n;
	vector<int> out = sol.grayCode(n);
	for (int r: out){
		cout << r << ' ';
	}cout << endl;
	return 0;
}
