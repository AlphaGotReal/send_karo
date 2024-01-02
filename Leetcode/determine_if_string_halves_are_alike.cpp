#include <iostream>

using namespace std;

class Solution{
	public:
		bool halvesAreALike(string s){
			string vowels = "aeiouAEIOU";
			int map[200] = {0};
			for (int t = 0; t < vowels.length(); t++){
				map[vowels[t]]++;
			}int v = 0, n = s.length();
			for (int t = 0; t < n/2; t++){
				if (map[s[t]]){
					v++;
				}if (map[s[n - t - 1]]){
					v--;
				}
			}return (v == 0);
		}
};

int main(){
	Solution sol;
	string s = "textbook";
	cout << sol.halvesAreALike(s) <<'\n';
	return 0;
}

