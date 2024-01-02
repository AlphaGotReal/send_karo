#include <bits/stdc++.h>

using namespace std;

class Solution{
public:
    bool backspaceCompare(string sstr, string tstr){
        stack<char> S, T;
        for (char s: sstr){
            if (s == '#' && !S.empty()){
                S.pop();
                continue;
            }
            S.push(s);
        }for (char t: tstr){
            if (t == '#' && !T.empty()){
                T.pop();
                continue;
            }
            T.push(t);
        }

        while (!S.empty()){
            if (T.empty()) return false;
            if (T.top() != S.top()) return false;
            T.pop(); S.pop();
        }return T.empty();

    }
};

int main(){

    return 0;
}


