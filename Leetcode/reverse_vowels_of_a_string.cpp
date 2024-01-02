#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	string reverseVowels(string s){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		string vowels = "";
		for (char r: s){
			if (this->isVowel(r))
				vowels += r;
		}

		int t = vowels.size()-1;

		string out = "";
		for (char r: s){
			if (this->isVowel(r)){
				out += vowels[t--];
				continue;
			}out += r;
		}return out;
	}

	bool isVowel(char r){
		if (r < 96) r = r + 32;
		return (r == 'a' || r == 'e' || r == 'i' || r == 'o' || r == 'u');
	}
};


