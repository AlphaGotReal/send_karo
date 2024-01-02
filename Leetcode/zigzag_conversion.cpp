#include <iostream>

using namespace std;

class Solution{
	public:
		string convert(string s, int n){
			int x = 0, len, inc = 1;
			int c[n+1]; c[0] = 0;
		
			for (len = 0; s[len]; len++){
				c[x+1]++;
				x += inc;
				if (x == 0 || x == n-1) inc = -inc;
			}
			
			for (int t = 1; t <= n; t++)
				c[t] += c[t-1];

			char ne[len];
			inc = 1;

			for (int y = 0, r = 0; r < len; r++){
				ne[c[y]++] = s[r];
				y += inc;
				if (y == 0 || y == n-1) inc = -inc;
			}

			string out = ne;
			return out;
		}
};

int main(){
	Solution sol;
	string s = "PAYPALISHIRING";
	cout << sol.convert(s, 5) <<'\n';


}


