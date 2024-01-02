#include <iostream>

using namespace std;

class Solution{
	public:
		int lengthOfLongestSubstring(string s){
			int max_chars = 200;
			
			int last_occurance[max_chars];
			for (int t = 0; t < max_chars; t++) last_occurance[t] = -1;

			int max_len = 0, sub_len = 0, check_start = 0;
			for (int r = 0; r < s.length(); r++){
				if (last_occurance[s[r]] >= check_start){
					if (max_len < sub_len) {
						max_len = sub_len;
					}check_start = last_occurance[s[r]] + 1;
					sub_len = r - last_occurance[s[r]] - 1;
				}last_occurance[s[r]] = r;
				sub_len++;
			}
			
			if (sub_len > max_len){
				return sub_len;
			}return max_len;

		}
};

int main(){
	string s = "bbbbbb";
	Solution t;
	cout << t.lengthOfLongestSubstring(s) << '\n';
	return 0;
}


