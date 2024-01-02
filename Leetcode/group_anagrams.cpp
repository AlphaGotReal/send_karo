#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	vector<vector<string>> groupAnagrams(vector<string>& strs) {
		unordered_map<string, vector<string>> mp;

		for(string str: strs){
			string word = str;
			sort(word.begin(), word.end());
			mp[word].push_back(str);
		}

		vector<vector<string>> ans;
		for(auto group: mp){
			ans.push_back(group.second);
		}
		return ans;
	}

};


int main(){

	Solution sol;
	vector<string> strs {"eat", "tea", "tan", "ate", "nat", "bat"};
	vector<vector<string>> groups = sol.groupAnagrams(strs);
	for (vector<string> group: groups){
		for (string str : group){
			cout << str << ' ';
		}cout << endl;
	}

	return 0;
}

