#include <stdio.h>

int maxArea0(int* height, int n){
	int max = 0;
	for (int y = 0; y < n; y++){
		for (int x = y + 1; x < n; x++){
			int m = height[y];
			if (m > height[x]){
				m = height[x];
			}if (max < m*(x-y)){
				max = m*(x-y);
			}
		}
	}return max;
}

int maxArea(int* height, int n){
	int max = 0, area, m;
	int rscan[n];
	int lscan[n];
	
	int r = 0, l = 0;
	
	rscan[r] = 0;
	lscan[l] = n-1;

	for (int t = 1; t < n; t++){
		if (height[rscan[r]] < height[t]){
			r++;
			rscan[r] = t;
		}if (height[lscan[l]] < height[n-t-1]){
			l++;
			lscan[l] = n-t-1;
		}
	}

	int R = 0, L = 0;

	for (int R = 0; R <= r; R++){
		for (int L = 0; L <= l; L++){
			if (rscan[R] >= lscan[L]) break;
			m = height[rscan[R]];
			if (m > height[lscan[L]]){
				m = height[lscan[L]];
			}area = m * (lscan[L] - rscan[R]);
			if (area > max){
				max = area;
			}
		}
	}return max;
}

int main(){
	int h[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
	int *height = h;
	int n = 9;
	//maxArea(height, n);
	printf("%d\n", maxArea(height, n));
	return 0;
}


