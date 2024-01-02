#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<vector<int>> merge(vector<vector<int>> &in){
		
		vector<vector<int>> intervals;
		priority_queue<pair<int, int>> out;
		for (int r = 0; r < in.size(); r++){
			out.push(pair<int, int>(-in[r][0], -in[r][1]));
		}

		while (!out.empty()){
			pair<int, int> p = out.top();
			intervals.push_back({-p.first, -p.second});
			out.pop();
		}
	
		//return intervals;	
		vector<vector<int>> answer;
		answer.push_back(intervals[0]);
		int last = 0;
		for (int t = 1; t < intervals.size(); t++){
			if (answer[last][1] >= intervals[t][0]){
				answer[last][1] = max(intervals[t][1], answer[last][1]);
			}else {
				answer.push_back(intervals[t]);
				last++;
			}
		}
		return answer;
	}
};

int main(){
	vector<vector<int>> intervals = {
		{1, 4},
		{0, 4}
	};

	Solution sol;
	vector<vector<int>> answer = sol.merge(intervals);

	for (int r = 0; r < answer.size(); r++){
		cout << answer[r][0] << ", " << answer[r][1] << endl;
	}

	return 0;
}


