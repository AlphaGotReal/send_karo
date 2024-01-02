#include <stdio.h>
#include <stdbool.h>

bool isMatch(char * s, char * p){
	int head = 0;
	for (int r = 0; s[r]; r++){
		if (s[r] != p[r-head]){
			if (p[r-head] != '?' && p[r-head] != '*'){
				return false;
			}
		}
	}return true;
}

int main(){
	char * s = "aa";
	char * p = "a?";
	printf("%d\n", isMatch(s, p));
}


