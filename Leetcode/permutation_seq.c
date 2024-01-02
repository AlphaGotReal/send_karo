#include <stdio.h>
#include <stdlib.h>

int fac(int x){
    if (x < 2) return 1;
    return x * fac(x-1);
}

int *arange(int n){
    int *arr = (int *)malloc(sizeof(int)*n);
    for (int t = 1; t <= n; t++) arr[t-1] = t;
    return arr;
}

void swap(int *a, int *b){
    int *t = a;
    *a = *b;
    b = t;
}

char getPermutation(int n, int k){
    if (n == 0) {
        printf("\n");
        return '\0';
    }
    static int fac_n = fac(n);
    static int *arr = arange(n);
    static int top = 0;
    fac_n = fac_n/n;
    int first = k/fac_n;
    printf("%d", first);
    int rest = k%fac_n;
    swap(&arr[first], &arr[top++]);
    return getPermutation(n-1, rest);
}

int main(){
    getPermutation(3, 3);
    return 0;
}

