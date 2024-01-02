#include <stdio.h>

int nCr(int n, int r){
	int t = 1, d = 1;
	for (int a = n - r + 1; a <= n; a++){
		t = t * a;
		d = d * (n - a + 1);
	}return t/d;
}

int main(){

}


