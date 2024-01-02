#include <stdio.h>

char* addBinary(char* a, char* b){
	int ia = 0, ib = 0;

	char *t;
	while (a[ia] || b[ib]){
		if (a[ia]) ia++;
		if (b[ib]) ib++;
	}

	if (ia > ib) {
		t = b;
		b = a;
		a = t;
		ia = ia + ib;
		ib = ia - ib;
		ia = ia - ib;
	}

	char carry = '0';

	while (ia >= 0 || ib >= 0){
		ia--;ib--;
		if (ia > 0 && ib > 0){
			add(&a[ia], &b[ib]);
			add(&carry, &b[ib]);
			if (carry == '1' || a[ia] == '1'){
				carry = '1';
			}
		}else{
			add(&carry, &b[ib]);
		}
	}return b;

}

void add(char *x, char *y){
	if (*x == '1' && *y == '1'){
		*x = '1'; 
		*y = '0';
	}else if (*x == '0' && *y == '0'){
		*x = '0';
		*y = '0';
	}else{
		*x = '0';
		*y = '1';
	}
}


int main(){
	char x[10] = "1";
	char y[10] = "1";
	printf("%s\n", addBinary(x, y));
	return 0;
}	


