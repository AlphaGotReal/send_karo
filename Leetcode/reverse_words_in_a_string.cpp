#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	string reverseWords(string s){
		ios_base::sync_with_stdio(0);
		cin.tie(0);

		vector<string> words;
		string temp = "";
		for (char r: s){
			if (r == ' '){
				if (temp != "") words.push_back(temp);
				temp = "";
				continue;
			}temp += r;
		}

		if (temp != ""){
			words.push_back(temp);
		}

		string last = words[words.size()-1];
		words.pop_back();

		string out = "";
		for (string word: words){
			out = " " + word + out;
		}
		return last + out;
	}
};


