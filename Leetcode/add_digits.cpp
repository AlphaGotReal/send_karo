#include <iostream>

class Solution{
	public:
		int addDigits0(int num){
			int t;
			while (num/10 != 0){
				t = 0;
				while (num){
					t += num%10;
					num = num/10;
				}num = t;
			}return num;
		}

		int addDigits(int num){
			
		}
};

int main(){
	Solution sol;
	int x;
	std::cin >> x;
	std::cout << sol.addDigits(x) << '\n';
	return 0;
}


