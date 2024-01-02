#include <iostream>

using namespace std;

class Solution{
	public:
		int strStr(string haystack, string needle){
			int start = 0;
			int index = 0;
			for (int r = 0; r < haystack.size(); r++){
				if (haystack[r] == needle[index]){
					index ++;
					if (index == needle.size()){
						return start;
					}continue;
				}r = r - index;
				index = 0;
				start ++;
			}return -1;
		}
};

int main(){
	Solution s;
	cout << s.strStr("sadleetcode", "lee") << '\n';
}


