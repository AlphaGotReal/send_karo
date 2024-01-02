#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	string mergeAlternately(string word1, string word2){
		
		ios_base::sync_with_stdio(0);
		cin.tie(0);

		string out = "";
		int n1 = word1.length();
		int n2 = word2.length();

		int p1 = 0, p2 = 0;

		for (int t = 0; t < max(n1, n2); ++t){
			if (p1 < n1){
				out = out + word1[p1++];
			}if (p2 < n2){
				out = out + word2[p2++];
			}
		}return out;
	}
			
};

int main(){

	return 0;
}


