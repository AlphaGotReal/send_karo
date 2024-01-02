#include <iostream>

using namespace std;

class Solution{
	public:
		int myAtoi(string s){
			int n = 0;
			int start = 0;
			int sign = 1;		
			int max = 2147483647;

			for (int r = 0; r < s.length(); r++){

				if (s[r] >= 48 && s[r] <= 57){
					start = 1;
					if (n <= (max - s[r] + 48) / 10)
						n = n*10 + s[r]-48;
					else
						if (sign == 1)
							return max;
						else
							return sign-max;
					continue;
				}

				if (s[r] != ' ' && !start){
					if (s[r] == '-' && s[r+1] >= 48 && s[r+1] <= 57) {
						sign = -1;
						continue;
					}if (s[r] == '+' && s[r+1] >= 48 && s[r+1] <= 57) continue;
					return 0;
				}

				if (start){
					return n*sign;
				}
			}return n*sign;
		}
};

int main(){
	Solution s;
	cout << s.myAtoi("21474836460") << '\n';
	return 0;
}

