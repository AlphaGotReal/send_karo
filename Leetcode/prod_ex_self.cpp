#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
    vector<int> productExceptSelf(vector<int> &arr){
        vector<int> out(arr.size(), 0);
        int pre = 1;
        int post = 1;
        for (int t = 0; t < arr.size(); t++){
            out[t] = pre;
            pre = pre * arr[t];
        }for (int t = arr.size()-1; t >= 0; t--){
            out[t]  = out[t] * post;
            post = post * arr[t];
        }return out;
    }
};

int main(){

    vector<int> arr = {1, 2, 3, 4};
    Solution sol;
    vector<int> out = sol.productExceptSelf(arr);
    for (int t: out){
        cout << t << endl;
    }

    return 0;
}


