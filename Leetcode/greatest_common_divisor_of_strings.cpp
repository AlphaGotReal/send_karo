#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	string gcdOfStrings(string a, string b){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		if (a+b != b+a) return "";
		int na = a.length();
		int nb = b.length();
		int l = this->gcd(na, nb);
		string g = "";
		for (int t = 0; t < l; ++t){
			if (a[t] != b[t]){
				return "";
			}g += a[t];
		}return g;
	}	

	int gcd(int a, int b){
		if (a < b) swap(a, b);
		if (a%b == 0) return b;
		return gcd(a%b, b);
	}
};

