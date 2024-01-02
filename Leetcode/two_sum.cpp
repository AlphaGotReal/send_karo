#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
    vector<int> twoSum(vector<int> &arr, int target){
        unordered_map<int, int> map;
        int n = arr.size();
        for (int t = 0; t < n; t++){
            int other = target - arr[t];
            if (map.count(other)){
                return {map[other], t};
            }map[arr[t]] = t;
        }return {};
    }
};

int main(){

    return 0;
}

