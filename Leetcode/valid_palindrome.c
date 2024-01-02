#include <stdio.h>
#include <stdbool.h>

bool isPalindrome(char * s){
	int n = 0;
	for (; s[n]; n++);

	int right_headstart = 0;
	int left_headstart = 0;
	
	int t = 0, q;
	char a, b;

	while (1){
		if (t+left_headstart == n-t-1-right_headstart) return true;
		q = 0;
		a = s[t+left_headstart];
		b = s[n-t+1-right_headstart];
		
		if (a < 97) a = a + 32;
		if (b < 97) b = b + 32;
		
		if (a == b){
			t++;
			continue;
		}if (122 <= a || a >= 97 ){
				left_headstart++;
				q++;
		}if (122 <= b || b >= 97 ){
				right_headstart++;
				q++;
		}if (q == 0){
			return false;
		}

		if (122 >= a && a >= 97 && 122 >= b)
	}return true;

}

int main(){

}


