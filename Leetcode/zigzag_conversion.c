#include <stdio.h>

char * convert0(char * s, int n){
	int t = 0, x, inc = 1;
	char new[1000];
	for (int y = 0; y < n; y++){
		x = 0;
		for (int r = 0; s[r]; r++){
			if (y == x){
				new[t++] = s[r];
			}x = x + inc;
			if (x == 0 || x == n-1){
				inc = -inc;
			}
		}
	}
	char * out = new;
	return out;
}

char * convert(char * s, int n){

	int x = 0, len, inc = 1;
	int c[n+1]; c[0] = 0;

	for (len = 0; s[len]; len++){
		c[x+1]++;
		x = x + inc;
		if (x == 0 || x == n-1) inc = -inc;
	}

	for (int t = 1; t <= n; t++){
		c[t] += c[t-1];
	}

	char new[len];
	inc = 1;

	for (int y = 0, r = 0; r < len; r++){
		new[c[y]++] = s[r];
		y += inc;
		if (y == 0 || y == n-1) inc = -inc;
	}

	char * out = new;
	return out;
}

int main(){
	char * string = "PAYPALISHIRING";
	char * out = convert(string, 5);
	printf("%s\n", out);
}


