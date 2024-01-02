#include <iostream>

using namespace std;
class Solution{
	public:
		//bruteforce
		string longestPalindrome0(string s){
			int len = 0;
			int start = 0;
		
			int n = s.length();
			if (n == 1){
				return s;
			}


			for (int st = 0; st < n - 1; st++){
				for (int ln = n - st; ln > 0; ln--){
					int palin = 1;
					for (int r = 0; r < ln/2; r++){
						if (s[st + r] != s[st + ln - r - 1]){
							palin = 0;
						}
					}if (palin && ln > len){
						len = ln;
						start = st;
						break;
					}
				}
			}
			string sub = "";
			for (int r = start; r < start + len; r++){
				sub = sub + s[r];
			}return sub;
		}

		//works only for odd palindromes
		string longestPalindrome1(string s){
			int winglen = 0;
			int mid = 0;
			
			int n = s.length();			
			int wing;
			if (n == 1){
				return s;	
			}

			for (int m = 0; m < n; m++){
				wing = 1;
				while (1){
					if (m - wing < 0 && m + wing >= n) {
						break;
					}if (s[m - wing] != s[m + wing]){
						wing--;
						break;
					}wing++;
				}if (wing > winglen){
					mid = m;
					winglen = wing;
				}
			}string sub = "";
			for (int r = mid - winglen; r <= mid + winglen; r++){
				sub += s[r];
			}return sub;
		}

		//slow
		string longestPalindrome2(string s){
			int winglen = 0;
			int mid = 0;
			
			int n = s.length();			
			int wing;
			if (n == 1){
				return s;	
			}

			for (int m = 0; m < n; m++){
				wing = 1;
				while (1){
					if (m - wing < 0 && m + wing >= n) {
						break;
					}if (s[m - wing] != s[m + wing]){
						wing--;
						break;
					}wing++;
				}if (wing > winglen){
					mid = m;
					winglen = wing;
				}
				wing = 1;
				while (1){
					if (m - wing < 0 && m + 1 + wing >= n){
						break;
					}if (s[m - wing] != s[m + 1 + wing]){
						wing--;
						break;
					}wing++;
				}if (wing > winglen){
					mid = m;
					winglen = wing;
				}
			}string sub = "";
			for (int r = mid - winglen; r <= mid + winglen; r++){
				sub += s[r];
			}return sub;
			
		}
	
		string longestPalindrome(string s){
			
		}

};

int main(){
	string s = "civilwartestingwhetherthatnaptionoranynartionsoconceivedandsodedicatedcanlongendureWeareqmetonagreatbattlefiemldoftzhatwarWehavecometodedicpateaportionofthatfieldasafinalrestingplaceforthosewhoheregavetheirlivesthatthatnationmightliveItisaltogetherfangandproperthatweshoulddothisButinalargersensewecannotdedicatewecannotconsecratewecannothallowthisgroundThebravelmenlivinganddeadwhostruggledherehaveconsecrateditfaraboveourpoorponwertoaddordetractTgheworldadswfilllittlenotlenorlongrememberwhatwesayherebutitcanneverforgetwhattheydidhereItisforusthelivingrathertobededicatedheretotheulnfinishedworkwhichtheywhofoughtherehavethusfarsonoblyadvancedItisratherforustobeherededicatedtothegreattdafskremainingbeforeusthatfromthesehonoreddeadwetakeincreaseddevotiontothatcauseforwhichtheygavethelastpfullmeasureofdevotionthatweherehighlyresolvethatthesedeadshallnothavediedinvainthatthisnationunsderGodshallhaveanewbirthoffreedomandthatgovernmentofthepeoplebythepeopleforthepeopleshallnotperishfromtheearth";
	//string s = "helleh";
	Solution sol;
	cout << sol.longestPalindrome0(s) << '\n';
	cout << sol.longestPalindrome1(s) << '\n';
	cout << sol.longestPalindrome2(s) << '\n';
	return 0; 
}

