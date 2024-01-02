#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool containsDuplicates(int *arr, int n){
	for (int t = 0; t < n; t++){
		for (int r = t+1; r < n; r++){
			if (arr[t] == arr[r]) return true;
		}
	}return false;
}	

int main(){

	int arr[] = {1, 2, 3, 4};
	if (containsDuplicates(&arr[0], 4)){
		printf("true\n");
	}else {
		printf("false\n");
	}

	return 0;
}


