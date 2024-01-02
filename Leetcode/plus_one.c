#include <stdio.h>

int* plusOne(int* digits, int n){
	int extra = 0;
	if (digits[0] == 9){
		for (int t = n; t > 0; t--){
			digits[t] = digits[t-1];
		}digits[0] = 0;
		n++;
		digits[n] = '\0';
	}
	digits[n-1]++;
	while (n){
		n--;
		digits[n] += extra;
		extra = digits[n]/10;
		digits[n] = digits[n]%10;
	}
	return digits;
}

int main(){
	int digits[] = {9, 9, 9, 9};
	int n = 4;
	int *lol = plusOne(digits, n);
	return 0;
}



