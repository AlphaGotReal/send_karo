#include <iostream>

using namespace std;

class Solution{
	public:
		string intToRoman0(int num){
			string s = "";
			int val[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
			string roms[] = {"M", "CM", "D", "CD", "C", "XC", "L" , "XL", "X", "IX", "V", "IV", "I"};

			for (int t = 0; t < 13; t++){
				if (num/val[t] == 0){
					num = num%val[t];
					continue;
				}for (int r = 0; r < num/val[t]; r++){
					s = s + roms[t];
				}num = num%val[t];
			}
			return s;
		}

		string intToRoman(int n){
			if (n == 0) return "";
			string th[] = {"", "M", "MM", "MMM"};
			string hun[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
			string ten[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
			string one[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

			string s = "";

			s += th[n/1000];
			n = n%1000;
			s += hun[n/100];
			n = n%100;
			s += ten[n/10];
			n = n%10;
			s += one[n];
			return s;
		}

};

int main(){
	Solution s;
	cout << s.intToRoman(3999) << '\n';
}
