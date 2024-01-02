#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
	bool canPlaceFlowers(vector<int> &flowers, int n){
		ios_base::sync_with_stdio(0);
		cin.tie(0);
		for (int t = 0; t < flowers.size(); ++t){
			if (flowers[t]) continue;
			int a = (t >= 1) ? t-1 : 0;
			int b = (t+1 <= flowers.size()-1) ? t+1 : flowers.size()-1;
			if (flowers[a] == 0 && flowers[t] == 0 && flowers[b] == 0){
				flowers[t] = 1;
				n--;
			}if (n == 0) return true;
		}return (n == 0);
	}
};


