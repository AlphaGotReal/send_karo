#include <iostream>

class Solution{
	public:
		int reverse(int x){
			int max = 2147483647;
			int sign = 1;
			if (x < 0){
				x = -x;
				sign = -1;
				max--;
			}int rev = 0;
			while (x){
				if (rev > max/10) return 0;
				if (rev*10 > max - x%10) return 0;
				rev = rev*10 + x%10;
				x = x/10;	
			}return sign * rev;
		}
};

int main(){
	int n = -1463847412;
	Solution s;// = new Solution();
	std::cout << s.reverse(n) << '\n';
	return 0;
}


