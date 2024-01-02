#include <iostream>

using namespace std;

class Solution{
	public :
		bool isMatch(string s, string p){
			if (s == p) return true;
			int head = 0;
			for (int r = 0; r < s.length(); r++){
				if (s[r] != p[r-head]){
					if (p[r-head] != '.' && p[r-head] != '*'){
						return false;
					}if (p[r-head] == '*'){
							if (s[r] == s[r-head]){
								return false;
							}head++;
					}
				} 
			}return true;
		}
};

int main(){
	Solution sol;
	string s = "ab";
	string p = ".a";
	cout << sol.isMatch(s, p) << '\n';

}


