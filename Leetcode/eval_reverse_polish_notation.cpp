#include <bits/stdc++.h>

using namespace std;

class Solution{
public:

	unordered_map<string, int> integers;
	unordered_map<string, bool> istoken = {{"+", true}, {"-", true}, {"*", true}, {"/", true}};

	int evalRPN(vector<string> &tokens){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		stack<int> st;
		for (string r: tokens){
			if (this->istoken.count(r) != 0){
				int one = st.top(); st.pop();
				int two = st.top(); st.pop();
				int three;
				if (r == "+") three = one + two;
				else if (r == "-") three = two - one;
				else if (r == "*") three = one * two;
				else three = two / one;
				st.push(three);
				continue;
			}st.push(this->fromString(r));
			cout << st.top() << endl;
		}return st.top();
	}

	int fromString(string s){
		if (this->integers.count(s) != 0) return this->integers[s];
		int sign = 1;
		int start = 0;
		if (s[0] == '-'){
			start = 1;
			sign = -1;
		}
		int integer = 0;
		int n = s.length();
		for (int t = start; t < n; ++t){
			integer = integer * 10  + (int)(s[t] - '0'); 
		}this->integers[s] = integer*sign;
		return integer*sign;
	}
};

int main(){

	Solution sol;
	vector<string> tokens = {"4", "13", "5", "/", "+"};

	sol.evalRPN(tokens);
	return 0;
}

