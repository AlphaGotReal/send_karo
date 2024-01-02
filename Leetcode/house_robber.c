#include <stdio.h>

int rob(int* nums, int numsSize){
	if (numsSize == 0){
		return 0;	
	}if (numsSize == 1){
		return 1;
	}

	int one = nums[numsSize-1] + rob(nums, numsSize-2);
	int other = nums[numsSize-2] + rob(nums, numsSize-3);

	if (one > other){
		return other;
	}return two;
}




