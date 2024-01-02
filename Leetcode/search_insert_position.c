#include <stdio.h>

int searchInsert(int *nums, int n, int target){
	int right = n-1, left = 0;
	int mid = (right + left)/2;
	while (right >= left){
		mid = (right + left)/2;
		if (nums[mid] > target){
			right = mid - 1;
			continue;
		}if (nums[mid] < target){
			left = mid + 1;
			continue;
		}return mid;
	}if (nums[mid] > target)
		return mid;
	return mid + 1;
}

int main(){
	int arr[] = {1,2,3,5,6,7};
	int target = 10;
	printf("%d \n", searchInsert(arr, 6, target));
}


