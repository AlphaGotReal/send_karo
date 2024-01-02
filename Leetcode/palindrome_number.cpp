#include <iostream>
#include <cmath>

using namespace std;

class Solution{
	public:
		bool isPalindrome(int x){
			if (x < 0) return false;
			int y = x;
			int rev = 0;
			while (x){
				rev = rev*10 + x%10;
				x = x/10;
			}return (y == rev);
			
		}
};

int main(){

}


