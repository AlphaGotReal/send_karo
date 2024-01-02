#include <stdio.h>

int mySqrt(int x){
	if (x < 2){
		return x;
	}
	int r = x;
	if (x > 46340){
		r = 46340;
	}
	int l = 0;
	int mid = 0;
	while (r > l){
		mid = (r + l)/2;
		if (mid*mid <= x && (mid+1)*(mid+1) > x){
			return mid;
		}if (mid*mid > x){
			r = mid - 1;
		}else if (mid*mid < x){
			l = mid + 1;
		}
	}return r;
}

int main(){
	int x = 2147395599;
	printf("%d\n", mySqrt(x));
}


