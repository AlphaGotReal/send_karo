#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	int canCompleteCircuit(vector<int> &gas, vector<int>& cost){
		int tank = 0;
		int diff = 0;
		int start = 0;
		int n = gas.size();
		for (int t = 0; t < n; t++){
			diff += gas[t] - cost[t];
			tank += gas[t] - cost[t];
			if (tank < 0){
				tank = 0;
				start = t + 1;
			}
		}if (diff < 0) return -1;
		return start;
	}
};


int main(){
	vector<int> gas = {1, 2, 3, 4, 5};
	vector<int> cost = {3, 4, 5, 1, 2};

	Solution sol;

	cout << sol.canCompleteCircuit(gas, cost) << endl;

	return 0;
}


